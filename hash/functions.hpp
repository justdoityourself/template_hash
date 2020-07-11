/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include "polynomial.hpp"

#include "scalar_t/int.hpp"

namespace template_hash
{
	namespace functions
	{
        /*
            template_hash_[FEED BYTES]_[BLOCK BYTES]_[VERSION]
        */

        /*
            While testing the performance of these as inline function, with full optimizations ( RELEASE BUILD ). I found that the performance droped db 2x or more.
            Leaving them as a macro did not see the same drop... IDK why but this is what I measured.
        */

        #define th_8_32_1(X) template_hash::polynomial::_general_hash<uint64_t>(X, std::array<uint64_t, 4> { 8312461053245862996, 2062925084401512419, 8347604128837450101, 4884635396288653628 }, std::array<uint64_t, 4> { 6287624849880064552, 7342089213514467324, 2167580547557054533, 8553349112118993147 })

        #define th_8_16_1(X) template_hash::polynomial::_general_hash<uint64_t>(X, std::array<uint64_t, 2> { 8312461053245862996, 2062925084401512419 }, std::array<uint64_t, 2> { 6287624849880064552, 7342089213514467324 })

        template < typename T > auto th_16_32_1(const T& data)
        {
            using ii = scalar_t::uintv_t<uint64_t, 2>;

            constexpr std::array<ii, 2> poly{ ii(5510071552570399037, 8312461053245862996), ii(8347604128837459991,2062925084401512419) };
            constexpr std::array<ii, 2> iv{ ii(6287624849880064552, 7342089213514467324), ii(7615036822838911991,4616688971735116250) };

            return polynomial::_general_hash<ii>(data, poly, iv);
        }

        template < typename T > auto th_32_128_1(const T& data)
        {
            using ii = scalar_t::uintv_t<uint64_t, 4>;

            constexpr std::array<ii, 4> poly{    ii(5510071552570399037, 8312461053245862996, 246246042414002182, 6772597168347111779),
                                                        ii(8347604128837459991,2062925084401512419,8892248527409889256,1087533249662576637),
                                                        ii(2253896403829616664,3583660530011078752,8347604128837450101,4884635396288653628),
                                                        ii(7562572581247132442,1365706473444767778,9175513465616652412,3173396259388057629) };

            constexpr std::array<ii, 4> iv{      ii(6287624849880064552, 7342089213514467324, 436503270028263414, 4455390150565050468),
                                                        ii(7615036822838911991,4616688971735116250,5943667353290631665,5004646724832247663),
                                                        ii(2792523567209208038,6084996894412956278,3180095203642964332,3545077906438668307),
                                                        ii(2167580547557054533,8553349112118993147,7407326397308717619,1022658224357599207) };

            return polynomial::_general_hash<ii>(data, poly, iv);
        }
	}
}