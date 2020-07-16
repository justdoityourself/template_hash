/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <immintrin.h>

namespace avx2_int
{
	/*
		This is not a true scalar integer type or big int, instead it is used internally in a hash function to help vectorize when the compiler can't.
	*/

	class alignas(32) xint256_t
	{
	public:

		xint256_t() { }

		void Zero()
		{
			std::memset(this, 0, sizeof(xint256_t));
		}

		xint256_t(uint64_t ox) : xint256_t(ox, ox, ox, ox) {}

		xint256_t(uint64_t o1, uint64_t o2, uint64_t o3, uint64_t o4)
		{
			uint64_t* p = (uint64_t*)this;
			p[0] = o1;
			p[1] = o2;
			p[2] = o3;
			p[3] = o4;
		}

		xint256_t operator * (const xint256_t & r) const
		{
			xint256_t result;

			result.data = _mm256_mullo_epi32(data, r.data);

			return result;
		}

		xint256_t& operator *= (const xint256_t& r)
		{
			data = _mm256_mullo_epi32(data, r.data);

			return *this;
		}

		xint256_t operator + (const xint256_t& r) const
		{
			xint256_t result;

			result.data = _mm256_add_epi64(data, r.data);

			return result;
		}

		xint256_t& operator += (const xint256_t& r)
		{
			data = _mm256_add_epi64(data, r.data);

			return *this;
		}

		xint256_t operator ^ (const xint256_t& r) const
		{
			xint256_t result;

			result.data = _mm256_xor_si256(data, r.data);

			return result;
		}

		xint256_t& operator ^= (const xint256_t& r)
		{
			data = _mm256_xor_si256(data, r.data);

			return *this;
		}

		xint256_t& operator = (const xint256_t& r)
		{
			data = r.data;

			return *this;
		}

		__m256i data;
	};

	static_assert(sizeof(xint256_t) == 32);
}