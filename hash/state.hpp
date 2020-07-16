/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include "functions.hpp"



namespace template_hash
{
	namespace stateful
	{
		class State_8_32_1
		{
		public:

			static constexpr size_t byte_size = 32;
			using type = std::array<uint64_t, 4>;
			using byte_type = std::array<uint8_t, 4*8>;

			template < typename T, typename O > static inline void hash(const T& t, O& o)
			{
				th_8_32_1(t, *(type *)o.data());
			}		

			template < typename T > static inline byte_type hash(const T& t)
			{
				byte_type result;

				th_8_32_1(t, *(type*)result.data());

				return result;
			}

			template<typename T> void Update(const T& r)
			{
				if (-1 == itr)
					throw std::runtime_error("Alignment Error");

				if (r.size() % byte_size)
				{
					if (itr == 0)
						th_8_32_1(r, state);
					else
						th_8_32_1_finish(r, state);

					itr = -1;
				}
				else
				{
					if (itr++ == 0)
						th_8_32_1_begin(r, state);
					else
						th_8_32_1_update(r, state);
				}
			}

			void Finish(byte_type& finish)
			{
				if (itr != -1)
				{
					th_8_32_1_finish(std::string_view(""), state);
					itr = -1;
				}

				std::copy((uint8_t*)state.data(), (uint8_t*)(state.data() + state.size()), finish.data());
			}

			byte_type Finish()
			{
				byte_type finish;
				Finish(finish);

				return finish;
			}

			template < typename T > T FinishT()
			{
				T finish;

				if (itr != -1)
				{
					th_8_32_1_finish(std::string_view(""), state);
					itr = -1;
				}

				std::copy((uint8_t*)state.data(), (uint8_t*)(state.data() + state.size()), finish.data());

				return finish;
			}

			type state;
			size_t itr = 0;
		};

		class alignas(16) State_16_32_1
		{
		public:

			static constexpr size_t byte_size = 32;
			using type = std::array<sse_int::xint128_t, 2>;
			using byte_type = std::array<uint8_t, 2 * 16>;

			template < typename T, typename O > static inline void hash(const T& t, O& o)
			{
				thx_16_32_1(t, *(type*)o.data());
			}

			template < typename T > static inline byte_type hash(const T& t)
			{
				byte_type result;

				thx_16_32_1(t, *(type*)result.data());

				return result;
			}

			template<typename T> void Update(const T& r)
			{
				if (-1 == itr)
					throw std::runtime_error("Alignment Error");

				if (r.size() % byte_size)
				{
					if (itr == 0)
						thx_16_32_1(r, state);
					else
						thx_16_32_1_finish(r, state);

					itr = -1;
				}
				else
				{
					if (itr++ == 0)
						thx_16_32_1_begin(r, state);
					else
						thx_16_32_1_update(r, state);
				}
			}

			void Finish(byte_type& finish)
			{
				if (itr != -1)
				{
					thx_16_32_1_finish(std::string_view(""), state);
					itr = -1;
				}

				std::copy((uint8_t*)state.data(), (uint8_t*)(state.data()+state.size()), finish.data());
			}

			byte_type Finish()
			{
				byte_type finish;
				Finish(finish);

				return finish;
			}

			template < typename T > T FinishT()
			{
				T finish;

				if (itr != -1)
				{
					thx_16_32_1_finish(std::string_view(""), state);
					itr = -1;
				}

				std::copy((uint8_t*)state.data(), (uint8_t*)(state.data() + state.size()), finish.data());

				return finish;
			}

			type state;
			size_t itr = 0;
		};
	}
}
