/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <immintrin.h>

#include "scalar_t/int.hpp"

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

		xint128_t operator - (const xint128_t& r) const
		{
			xint128_t result;

			result.data = _mm_sub_epi64(data, r.data);

			return result;
		}

		xint128_t& operator -= (const xint128_t& r)
		{
			data = _mm_sub_epi64(data, r.data);

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

	class alignas(16) pint128_t
	{
	public:

		uint32_t MultiplicativeInverse()
		{
			return (uint32_t)scalar_t::uintv_t<uint32_t, 1>(data.m128i_u32[0]).MultiplicativeInverse()[0];
		}

		pint128_t() { }

		void Zero()
		{
			std::memset(this, 0, sizeof(pint128_t));
		}

		pint128_t(uint32_t ox) : pint128_t(ox, ox,ox,ox) {}

		pint128_t(uint32_t o1, uint32_t o2, uint32_t o3, uint32_t o4)
		{
			uint32_t* p = (uint32_t*)this;
			p[0] = o1;
			p[1] = o2;
			p[2] = o3;
			p[3] = o4;
		}

		pint128_t operator * (const pint128_t& r) const
		{
			pint128_t result;

			result.data = _mm_mullo_epi32(data, r.data);

			return result;
		}

		pint128_t& operator *= (const pint128_t& r)
		{
			data = _mm_mullo_epi32(data, r.data);

			return *this;
		}

		pint128_t operator + (const pint128_t& r) const
		{
			pint128_t result;

			result.data = _mm_add_epi32(data, r.data);

			return result;
		}

		pint128_t& operator += (const pint128_t& r)
		{
			data = _mm_add_epi32(data, r.data);

			return *this;
		}

		pint128_t operator - (const pint128_t& r) const
		{
			pint128_t result;

			result.data = _mm_sub_epi32(data, r.data);

			return result;
		}

		pint128_t& operator -= (const pint128_t& r)
		{
			data = _mm_sub_epi32(data, r.data);

			return *this;
		}

		pint128_t operator ^ (const pint128_t& r) const
		{
			pint128_t result;

			result.data = _mm_xor_si128(data, r.data);

			return result;
		}

		pint128_t& operator ^= (const pint128_t& r)
		{
			data = _mm_xor_si128(data, r.data);

			return *this;
		}

		pint128_t& operator = (const pint128_t& r)
		{
			data = r.data;

			return *this;
		}

		bool operator == (const pint128_t& r)
		{
			__m128i temp = _mm_cmpeq_epi32(data, r.data);
			uint16_t mask = _mm_movemask_epi8(temp);

			return mask == 0xffff;
		}

		bool operator != (const pint128_t& r)
		{
			return !this->operator==(r);
		}

		__m128i data;
	};

	static_assert(sizeof(pint128_t) == 16);
}