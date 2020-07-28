/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <array>

#include "d8u/buffer.hpp"
#include "d8u/string.hpp"

namespace template_hash
{
	namespace polynomial
	{
		/*
			Working example:

			hash "test" with quadratic polynomial {17, 99, 14}

			UNIT = byte
			BLOCK = byte

						   |
			116 101 115 116
				25	79	204
					248	119	94
						239 70 144
							37 253 18
						   |

			Colapse context 37 ^ 253 ^ 18 = 202
		*/

		/*
			UNIT is how data will be divided.
			BLOCK is where the data will accumulate.
			POLYNOMIAL is a compile time array of the cofficients.

			Alignment is achieved by extending last unit with 0 and repeating the last unit to cover one block.

			This function returns the entire hash context.
			TODO allow the context to be input so we can resume a blocked hashing process.
		*/

		template < typename UNIT, typename T, typename BLOCK, size_t COUNT > void _general_hash(const T& _data, const std::array<BLOCK, COUNT>& poly, std::array<BLOCK, COUNT> & result,const std::array<BLOCK, COUNT>& IV = {})
		{
			auto data = d8u::byte_buffer(_data);

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT);
			size_t tail = data.size() % sizeof(UNIT);

			UNIT left_over(7764590573423603017);

			if (tail)
				std::memcpy(&left_over,(uint8_t*)(u + limit),tail);

			for (size_t i = 0; i < COUNT; i++)
				result[i] = IV[i];

			for (size_t i = 0; i < COUNT && i < limit; i++)
				result[i] ^= u[i];

			for (size_t i = limit; i < COUNT; i++)
				result[i] ^= left_over;

			for (size_t i = 0; i + COUNT < limit; i++)
			{
				auto head = result[0];
				for (size_t j = 0; j < COUNT-1; j++)
					result[j] = head * poly[j] + result[j + 1];

				result[COUNT - 1] = head * poly[COUNT - 1] + u[i + COUNT];
			}

			for (size_t i = 0; i < COUNT; i++)
			{
				auto head = result[0];

				for (size_t j = 0; j < COUNT - 1; j++)
					result[j] = head * poly[j] + result[j + 1];

				result[COUNT - 1] = head * poly[COUNT - 1] + left_over;
			}
		}

		/*
			Does this function run faster in some cases? Looks to be pretty much the same.
		*/

		/*template < typename UNIT, typename BLOCK, typename T, size_t COUNT > auto _general_hash_single_loop(const T& data, const std::array<BLOCK, COUNT>& poly, const std::array<BLOCK, COUNT>& IV = {})
		{
			std::array<BLOCK, COUNT> result = {};

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT);
			size_t tail = data.size() % sizeof(UNIT);

			UNIT left_over = 0;

			if (tail)
			{
				uint8_t* byte_i = (uint8_t*)(u + limit);

				for (size_t i = 0; i < tail; i++)
				{
					left_over <<= 8;
					left_over += *byte_i++;
				}
			}

			for (size_t i = 0; i < COUNT && i < limit; i++)
				result[i] = IV[i] ^ u[i];

			{
				size_t i = 0;

				if (tail)
					limit++;

				for (; i < limit; i++)
				{
					auto head = result[0];

					for (size_t j = 0; j < COUNT - 1; j++)
						result[j] = head * poly[j] + result[j + 1];

					result[COUNT - 1] = head * poly[COUNT - 1] + ((i + COUNT < limit) ? u[i + COUNT] : left_over);
				}
			}

			return result;
		}*/

		/*
			Does unrolling help? not that I can tell.
		*/

		/*template < typename UNIT, typename BLOCK, typename T > auto _general_hash_single_loop(const T& data, const std::array<BLOCK, 4>& poly, const std::array<BLOCK, 4>& IV = {})
		{
			std::array<BLOCK, 4> result = {};

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT);
			size_t tail = data.size() % sizeof(UNIT);

			UNIT left_over = 0;

			if (tail)
			{
				uint8_t* byte_i = (uint8_t*)(u + limit);

				for (size_t i = 0; i < tail; i++)
				{
					left_over <<= 8;
					left_over += *byte_i++;
				}
			}

			result[0] = IV[0] ^ u[0];
			result[1] = IV[1] ^ u[1];
			result[2] = IV[2] ^ u[2];
			result[3] = IV[3] ^ u[3];

			{
				size_t i = 0;
				for (; i + 4 < limit; i++)
				{
					auto head = result[0];

					result[0] = head * poly[0] + result[1];
					result[1] = head * poly[1] + result[2];
					result[2] = head * poly[2] + result[3];

					result[4 - 1] = head * poly[4 - 1] + u[i + 4];
				}

				if (tail)
					limit++;

				for (; i < limit; i++)
				{
					auto head = result[0];

					result[0] = head * poly[0] + result[1];
					result[1] = head * poly[1] + result[2];
					result[2] = head * poly[2] + result[3];

					result[4 - 1] = head * poly[4 - 1] + ((i + 4 < limit) ? u[i + 4] : left_over);
				}
			}

			return result;
		}*/


		/*
			This function finalizes the hash into a single block, if that is what you want. Returning that result.
		*/

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT  > BLOCK general_hash(const T& data, const std::array<BLOCK, COUNT>& poly, const std::array<BLOCK, COUNT>& IV = {})
		{
			BLOCK result = 0;

			std::array<BLOCK, COUNT> context;
			_general_hash<UNIT>(data, poly,context,IV);

			for (auto& i : context)
				result ^= i;

			return result;
		}



		/*
			Segmented hashing must happen aligned to the feed UNIT until the finish routine.
			The first block must also have UNIT * COUNT bytes to create the context.
		*/

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT > auto general_hash_begin(std::array<BLOCK, COUNT> & context,const T& _data, const std::array<BLOCK, COUNT>& poly, const std::array<BLOCK, COUNT>& IV = {})
		{
			auto data = d8u::byte_buffer(_data);

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT);

			if (data.size() % sizeof(UNIT))
				throw std::runtime_error("Alignment Failure");

			if(COUNT > limit)
				throw std::runtime_error("Underflow");

			for (size_t i = 0; i < COUNT && i < limit; i++)
				context[i] = IV[i] ^ u[i];
	
			for (size_t i = 0; i + COUNT < limit; i++)
			{
				auto head = context[0];
				for (size_t j = 0; j < COUNT - 1; j++)
					context[j] = head * poly[j] + context[j + 1];

				context[COUNT - 1] = head * poly[COUNT - 1] + u[i + COUNT];
			}
		}

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT > auto general_hash_continue(std::array<BLOCK, COUNT>& context, const T& _data, const std::array<BLOCK, COUNT>& poly)
		{
			auto data = d8u::byte_buffer(_data);

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT) + COUNT;
			
			if (data.size() % sizeof(UNIT))
				throw std::runtime_error("Alignment Failure");

			size_t i = 0;
			for (; i + COUNT < limit; i++)
			{
				auto head = context[0];
				for (size_t j = 0; j < COUNT - 1; j++)
					context[j] = head * poly[j] + context[j + 1];

				context[COUNT - 1] = head * poly[COUNT - 1] + u[i];
			}
		}

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT > auto general_hash_finish(std::array<BLOCK, COUNT>& context, const T& _data, const std::array<BLOCK, COUNT>& poly)
		{
			auto data = d8u::byte_buffer(_data);

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT) + COUNT;
			size_t tail = data.size() % sizeof(UNIT);

			UNIT left_over(7764590573423603017);

			if (tail)
				std::memcpy(&left_over, (uint8_t*)(u + limit - COUNT), tail);

			for (size_t i = 0; i + COUNT < limit; i++)
			{
				auto head = context[0];
				for (size_t j = 0; j < COUNT - 1; j++)
					context[j] = head * poly[j] + context[j + 1];

				context[COUNT - 1] = head * poly[COUNT - 1] + u[i];
			}

			for (size_t i = 0; i < COUNT; i++)
			{
				auto head = context[0];

				for (size_t j = 0; j < COUNT - 1; j++)
					context[j] = head * poly[j] + context[j + 1];

				context[COUNT - 1] = head * poly[COUNT - 1] + left_over;
			}
		}
	}
}