/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <string>
#include <array>
#include <chrono>

#include "polynomial.hpp"
#include "distribution.hpp"
#include "functions.hpp"

#include "d8u/hash.hpp"


TEST_CASE("scalar_t", "[hash::]")
{
    using namespace std::chrono;
    using namespace template_hash::distribution;
    using namespace template_hash::polynomial;
    using namespace template_hash::functions;

    constexpr auto reps = 10;

    auto map = mio::mmap_source("data/words_alpha.txt"/*,0,64*1024*/);

    //Load all pages into memory
    std::vector<uint8_t> file(map.size());
    std::copy(map.begin(), map.end(), file.begin());

    constexpr std::array<uint64_t, 4> poly{ 8312461053245862996,2062925084401512419, 8347604128837450101,4884635396288653628 };
    constexpr std::array<uint64_t, 4> iv{ 6287624849880064552, 7342089213514467324,2167580547557054533,8553349112118993147 };

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for(size_t i = 0; i < reps; i++)
        auto control = _general_hash<uint64_t>(map, poly, iv);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto control = _general_hash<uint64_t>(map, poly, iv);
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST1 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control.data(), 32)) << std::endl << std::endl;

 
   /* t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control = _general_hash_single_loop<uint64_t>(file, poly, iv);

    t2 = high_resolution_clock::now();
    control = _general_hash_single_loop<uint64_t>(file, poly, iv);
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST1 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control.data(), 32)) << std::endl << std::endl;*/


    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control = _general_hash<uint64_t>(file, poly, iv);

    t2 = high_resolution_clock::now();
    control = _general_hash<uint64_t>(file, poly, iv);
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST1 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control.data(), 32)) << std::endl << std::endl;


    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control = th_8_32_1(map); // For some reason when this was a function performanced dropped by huge amount. Not a problem as macro :( ?

    t2 = high_resolution_clock::now();
    control = th_8_32_1(map);
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST1 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control.data(), 32)) << std::endl << std::endl;



    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control2 = _general_hash<uint16_t>(map, std::array<uint64_t, 10>{7321, 199995061177, 6113, 4657, 1999934317, 8893, 17321, 199995068531, 23197, 314});

    t2 = high_resolution_clock::now();
    auto control2 = _general_hash<uint16_t>(map, std::array<uint64_t, 10>{7321, 199995061177, 6113, 4657, 1999934317, 8893, 17321, 199995068531, 23197, 314});
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST2 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control2.data(), 80)) << std::endl << std::endl;



    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control3 = _general_hash<uint32_t>(map, std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});

    t2 = high_resolution_clock::now();
    auto control3 = _general_hash<uint32_t>(map, std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST2 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control3.data(), 28)) << std::endl << std::endl;



    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        auto control4 = th_8_16_1(map);

    t2 = high_resolution_clock::now();
    auto control4 = th_8_16_1(map);;
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "TEST3 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)control4.data(), 16)) << std::endl << std::endl;


    t1 = high_resolution_clock::now();

    for (size_t i = 0; i < reps; i++)
        d8u::transform::DefaultHash h(map);

    t2 = high_resolution_clock::now();

    d8u::transform::DefaultHash h(map);
    std::cout << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
    std::cout << "SHA256 " << d8u::util::to_hex(gsl::span<uint8_t>((uint8_t*)h.data(), 32)) << std::endl << std::endl;

    CHECK(true);
}


//TEST_CASE("Segmented Hash Start + Continue + Finish", "[hash::]")
//{
//    using namespace std::chrono;
//    using namespace template_hash::distribution;
//    using namespace template_hash::polynomial;
//
//    auto poly = std::array<uint16_t, 4>{37, 9991, 17311, 4523};
//    std::array<uint16_t, 4> context;
//
//    general_hash_begin<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_continue<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_finish<uint8_t>(context, std::string_view("3"), poly);
//
//    auto control = _general_hash<uint8_t>(std::string_view("testtest3"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//}
//
//
//TEST_CASE("Segmented Hash Start + Finish + Odd", "[hash::]")
//{
//    using namespace std::chrono;
//    using namespace template_hash::distribution;
//    using namespace template_hash::polynomial;
//
//    auto poly = std::array<uint16_t, 4>{37, 9991, 17311, 4523};
//    std::array<uint16_t, 4> context;
//    general_hash_begin<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_finish<uint8_t>(context, std::string_view("s"), poly);
//
//    auto control = _general_hash<uint8_t>(std::string_view("tests"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//    general_hash_begin<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_finish<uint8_t>(context, std::string_view("ss"), poly);
//
//    control = _general_hash<uint8_t>(std::string_view("testss"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//
//    general_hash_begin<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_finish<uint8_t>(context, std::string_view("sss"), poly);
//
//    control = _general_hash<uint8_t>(std::string_view("testsss"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//
//    general_hash_begin<uint16_t>(context, std::string_view("testtest"), poly);
//
//    general_hash_finish<uint16_t>(context, std::string_view(""), poly);
//
//    control = _general_hash<uint16_t>(std::string_view("testtest"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//    general_hash_begin<uint16_t>(context, std::string_view("testtest"), poly);
//
//    general_hash_finish<uint16_t>(context, std::string_view("s"), poly);
//
//    control = _general_hash<uint16_t>(std::string_view("testtests"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//    general_hash_begin<uint16_t>(context, std::string_view("testtest"), poly);
//
//    general_hash_finish<uint16_t>(context, std::string_view("ss"), poly);
//
//    control = _general_hash<uint16_t>(std::string_view("testtestss"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//
//
//    general_hash_begin<uint16_t>(context, std::string_view("testtest"), poly);
//
//    general_hash_finish<uint16_t>(context, std::string_view("sss"), poly);
//
//    control = _general_hash<uint16_t>(std::string_view("testtestsss"), poly);
//
//    CHECK(std::equal(control.begin(), control.end(), context.begin()));
//}
//
//TEST_CASE("Segmented Hash Start + Finish", "[hash::]")
//{
//    using namespace std::chrono;
//    using namespace template_hash::distribution;
//    using namespace template_hash::polynomial;
//
//    auto poly = std::array<uint16_t, 4>{37, 9991, 17311, 4523};
//    std::array<uint16_t, 4> context;
//    general_hash_begin<uint8_t>(context, std::string_view("test"), poly);
//
//    general_hash_finish<uint8_t>(context, std::string_view(""), poly);
//
//    auto control = _general_hash<uint8_t>(std::string_view("test"), poly);
//
//    CHECK(std::equal(control.begin(),control.end(),context.begin()));
//}
//
//TEST_CASE("Distribution", "[hash::]")
//{
//    using namespace std::chrono;
//    using namespace template_hash::distribution;
//    using namespace template_hash::polynomial;
//
//    static auto constexpr words = 400000;
//    static auto constexpr buckets = 24000;
//
//    auto hash1 = [](auto word)
//    {
//        return general_hash<uint16_t>(word, std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});
//    }; 
//
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//
//    auto result1 = low_high_avg_tar<buckets>(hash1, words);
//
//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    std::cout << "4 byte 6th degree, deviation:" << std::get<2>(result1) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count()/1000 << std::endl;
//
//    auto hash2 = [](auto word)
//    {
//        return general_hash<uint8_t>(word,std::array<uint8_t, 3>{17, 99, 14});
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result2 = low_high_avg_tar<buckets>(hash2, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "1 byte quadratic, deviation:" << std::get<2>(result2) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//    auto hash5 = [](auto word)
//    {
//        return general_hash<uint16_t>(word, std::array<uint16_t, 3>{17, 99, 14});
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result5 = low_high_avg_tar<buckets>(hash5, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "2 byte quadratic, deviation:" << std::get<2>(result5) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//
//    auto hash6 = [](auto word)
//    {
//        return general_hash<uint8_t>(word, std::array<uint32_t, 1>{67737});
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result6 = low_high_avg_tar<buckets>(hash6, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "4 byte constant, deviation:" << std::get<2>(result6) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//
//    auto hash7 = [](auto word)
//    {
//        return general_hash<uint8_t>(word, std::array<uint16_t, 1>{63737});
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result7 = low_high_avg_tar<buckets>(hash7, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "2 byte constant, deviation:" << std::get<2>(result7) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//
//
//    auto hash3 = [](auto word)
//    {
//        d8u::transform::DefaultHash h(word);
//        return *(uint64_t*)&h;
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result3 = low_high_avg_tar<buckets>(hash3, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "8 byte SHA256, deviation:" << std::get<2>(result3) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//    auto hash4 = [](auto word)
//    {
//        return general_hash<uint16_t>(word, std::array<uint64_t, 10>{7321, 199995061177,6113, 4657, 1999934317, 8893, 17321, 199995068531, 23197, 314});
//    };
//
//    t1 = high_resolution_clock::now();
//    auto result4 = low_high_avg_tar<buckets>(hash4, words);
//    t2 = high_resolution_clock::now();
//
//    std::cout << "8 byte 9th degree, deviation:" << std::get<2>(result4) << " in " << (t2.time_since_epoch() - t1.time_since_epoch()).count() / 1000 << std::endl;
//
//    CHECK(true);
//}
//
//TEST_CASE("Polynomial Working Example", "[hash::]")
//{
//    using namespace template_hash::polynomial;
//
//    auto result = general_hash<uint8_t, uint8_t>(std::string("test"), std::array<uint8_t, 3>{17, 99, 14});
//
//    CHECK(202 == result);
//}
//
//TEST_CASE("Polynomial Larger", "[hash::]")
//{
//    using namespace template_hash::polynomial;
//
//    auto result = general_hash<uint8_t>(std::string("testtesttesttesttesttest"), std::array<uint32_t, 7>{7321, 6113, 4657, 1999934317, 8893, 17321, 23197});
//
//    CHECK(true);
//}