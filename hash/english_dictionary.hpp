/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include "../mio.hpp"
#include "d8u/buffer.hpp"

namespace template_hash
{
	namespace english
	{
		template < typename F > void get_words(F && f)
		{
			auto map = mio::mmap_source("data/words_alpha.txt");

			d8u::buffer::Helper stream(map);
			auto line = stream.GetLine();

			while (line.size())
			{
				if (!f(line))
					break;

				line = stream.GetLine();
			}
		}
	}
}