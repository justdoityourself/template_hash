/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <string>
#include <array>
#include <chrono>

#include "polynomial.hpp"
#include "distribution.hpp"

#include "d8u/hash.hpp"


TEST_CASE("Distribution", "[hash::]")
{
    using namespace std::chrono;
    using namespace template_hash::distribution;
    using namespace template_hash::polynomial;

    static auto constexpr words = 400000;
    static auto constexpr buckets = 24000;

    auto hash1 = [](auto word)
    {
        return general_hash<uint16_t>(word, std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});
    }; 

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    auto result1 = low_high_avg_tar<buckets>(hash1, words);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    std::cout << "4 byte 6th degree, deviation:" << std::get<2>(result1) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count()/1000 << std::endl;

    auto hash2 = [](auto word)
    {
        return general_hash<uint8_t>(word,std::array<uint8_t, 3>{17, 99, 14});
    };

    t1 = high_resolution_clock::now();
    auto result2 = low_high_avg_tar<buckets>(hash2, words);
    t2 = high_resolution_clock::now();

    std::cout << "1 byte quadratic, deviation:" << std::get<2>(result2) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;

    auto hash5 = [](auto word)
    {
        return general_hash<uint16_t>(word, std::array<uint16_t, 3>{17, 99, 14});
    };

    t1 = high_resolution_clock::now();
    auto result5 = low_high_avg_tar<buckets>(hash5, words);
    t2 = high_resolution_clock::now();

    std::cout << "2 byte quadratic, deviation:" << std::get<2>(result5) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;


    auto hash6 = [](auto word)
    {
        return general_hash<uint8_t>(word, std::array<uint32_t, 1>{67737});
    };

    t1 = high_resolution_clock::now();
    auto result6 = low_high_avg_tar<buckets>(hash6, words);
    t2 = high_resolution_clock::now();

    std::cout << "4 byte constant, deviation:" << std::get<2>(result6) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;


    auto hash7 = [](auto word)
    {
        return general_hash<uint8_t>(word, std::array<uint16_t, 1>{63737});
    };

    t1 = high_resolution_clock::now();
    auto result7 = low_high_avg_tar<buckets>(hash7, words);
    t2 = high_resolution_clock::now();

    std::cout << "2 byte constant, deviation:" << std::get<2>(result7) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;



    auto hash3 = [](auto word)
    {
        d8u::transform::DefaultHash h(word);
        return *(uint64_t*)&h;
    };

    t1 = high_resolution_clock::now();
    auto result3 = low_high_avg_tar<buckets>(hash3, words);
    t2 = high_resolution_clock::now();

    std::cout << "8 byte SHA256, deviation:" << std::get<2>(result3) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;

    auto hash4 = [](auto word)
    {
        return general_hash<uint16_t>(word, std::array<uint64_t, 10>{7321, 199995061177,6113, 4657, 1999934317, 8893, 17321, 199995068531, 23197, 314});
    };

    t1 = high_resolution_clock::now();
    auto result4 = low_high_avg_tar<buckets>(hash4, words);
    t2 = high_resolution_clock::now();

    std::cout << "8 byte 9th degree, deviation:" << std::get<2>(result4) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;

    CHECK(true);
}

TEST_CASE("Polynomial Working Example", "[hash::]")
{
    using namespace template_hash::polynomial;

    auto result = general_hash<uint8_t, uint8_t>(std::string("test"), std::array<uint8_t, 3>{17, 99, 14});

    CHECK(202 == result);
}

TEST_CASE("Polynomial Larger", "[hash::]")
{
    using namespace template_hash::polynomial;

    auto result = general_hash<uint8_t>(std::string("testtesttesttesttesttest"), std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});

    CHECK(true);
}