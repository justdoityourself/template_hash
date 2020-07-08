/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include "english_dictionary.hpp"

namespace template_hash
{
	namespace distribution
	{
		template < size_t MOD, typename HASH_FUNCTION > auto check(HASH_FUNCTION && f, size_t words = -1)
		{
			std::array<size_t, MOD> result = {};

			english::get_words([&](auto word)
			{
				auto h = f(word);
				result[h % MOD] ++;

				return --words != 0;
			});

			return result;
		}

		template < size_t MOD, typename HASH_FUNCTION > auto low_high_avg_tar(HASH_FUNCTION&& f, size_t words = -1)
		{
			auto result = check<MOD>(f, words);

			size_t low = -1;
			size_t high = 0;
			size_t deviation = 0;
			size_t sum = 0;
			auto tar = words / MOD;

			for (auto& i : result)
			{
				if (low > i)
					low = i;
				if (high < i)
					high = i;
				if(i > tar)
					sum += i-tar;
				else
					sum += tar-i;
			}

			return std::make_tuple(low, high, sum / MOD,tar,result);
		}
	}
}