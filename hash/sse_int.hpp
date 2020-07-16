/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <immintrin.h>

namespace sse_int
{
	/*
		This is not a true scalar integer type or big int, instead it is used internally in a hash function to help vectorize when the compiler can't.
	*/

	class alignas(16) xint128_t
	{
	public:

		xint128_t() { }

		void Zero()
		{
			std::memset(this, 0, sizeof(xint128_t));
		}

		xint128_t(uint64_t ox) : xint128_t(ox, ox) {}

		xint128_t(uint64_t o1, uint64_t o2)
		{
			uint64_t* p = (uint64_t*)this;
			p[0] = o1;
			p[1] = o2;
		}

		xint128_t operator * (const xint128_t& r) const
		{
			xint128_t result;

			result.data = _mm_mullo_epi32(data, r.data);

			return result;
		}

		xint128_t& operator *= (const xint128_t& r)
		{
			data = _mm_mullo_epi32(data, r.data);

			return *this;
		}

		xint128_t operator + (const xint128_t& r) const
		{
			xint128_t result;

			result.data = _mm_add_epi64(data, r.data);

			return result;
		}

		xint128_t& operator += (const xint128_t& r)
		{
			data = _mm_add_epi64(data, r.data);

			return *this;
		}

		xint128_t operator ^ (const xint128_t& r) const
		{
			xint128_t result;

			result.data = _mm_xor_si128(data, r.data);

			return result;
		}

		xint128_t& operator ^= (const xint128_t& r)
		{
			data = _mm_xor_si128(data, r.data);

			return *this;
		}

		xint128_t& operator = (const xint128_t& r)
		{
			data = r.data;

			return *this;
		}

		__m128i data;
	};

	static_assert(sizeof(xint128_t) == 16);
}