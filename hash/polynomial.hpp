/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <array>

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

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT > auto _general_hash(const T& data, const std::array<BLOCK, COUNT> & poly)
		{
			std::array<BLOCK, COUNT> result = {};

			UNIT* u = (UNIT*)data.data();
			size_t limit = data.size() / sizeof(UNIT);
			size_t tail = data.size() % sizeof(UNIT);

			UNIT left_over = 0;

			if (tail)
			{
				uint8_t* byte_i = (uint8_t*)(u+limit);

				for (size_t i = 0; i < tail; i++)
				{
					left_over <<= 8;
					left_over += *byte_i++;
				}
			}

			for (size_t i = 0; i < COUNT && i < limit; i++)
				result[i] = u[i];

			{
				size_t i = 0;
				for (; i + COUNT < limit; i++)
				{
					auto head = result[0];
					for (size_t j = 0; j < COUNT-1; j++)
						result[j] = head * poly[j] + result[j + 1];

					result[COUNT - 1] = head * poly[COUNT - 1] + u[i + COUNT];
				}

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
		}


		/*
			This function finalizes the hash into a single block. Returning that result.
		*/

		template < typename UNIT, typename BLOCK, typename T, size_t COUNT  > BLOCK general_hash(const T& data, const std::array<BLOCK, COUNT>& poly)
		{
			BLOCK result = 0;
			auto context = _general_hash<UNIT, BLOCK>(data, poly);

			for (auto& i : context)
				result ^= i;

			return result;
		}
	}
}