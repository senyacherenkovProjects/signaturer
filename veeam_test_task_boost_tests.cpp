//#define BOOST_TEST_MODULE MyTest
//#include <boost/test/included/unit_test.hpp> //single-header

#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <string>
#include <iterator>
#include <algorithm>

#include "signaturer.h"

bool compareFiles(const std::string& p1, const std::string& p2) {
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

	if (f1.fail() || f2.fail()) {
		return false; //file problem
	}

	if (f1.tellg() != f2.tellg()) {
		return false; //size mismatch
	}

	//seek back to beginning and use std::equal to compare contents
	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(f2.rdbuf()));
}

void file_generation(const std::string_view& fn, std::size_t n = 20000)
{
	auto gen = std::mt19937{ std::random_device{}() };
	std::uniform_int_distribution<unsigned> dist(1'000'000, 2'000'000);

	auto file = std::ofstream{ fn.data() }; // NOLINT

	for (std::size_t i = 0; i < n; ++i) {
		file << dist(gen) << '\n';
	}
}

//BOOST_AUTO_TEST_CASE(check_equality_for_two_output_files)
//{
//	auto& signaturer = Signaturer::GetInstance();
//
//	std::string infn{ "input.txt" };
//	std::string outfn{ "output.txt" };
//
//	file_generation(infn);
//	signaturer.SetInputFileName(infn);
//	signaturer.SetOutputFileName(outfn);
//
//	signaturer.Run(); //fill output
//
//	std::string outfn2{ "output2.txt" };
//	signaturer.SetOutputFileName(outfn2);
//
//	signaturer.Run(); //fill output2
//
//	BOOST_CHECK(compareFiles(outfn, outfn2));
//}
//
//BOOST_AUTO_TEST_CASE(check_equality_for_two_output_files_for_data_multiple_to_chunk_size)
//{
//	auto& signaturer = Signaturer::GetInstance();
//
//	std::string infn{ "input.txt" };
//	std::string outfn{ "output.txt" };
//
//	file_generation(infn);
//	signaturer.SetBlockSize(1000);
//	signaturer.SetInputFileName(infn);
//	signaturer.SetOutputFileName(outfn);
//
//	signaturer.Run(); //fill output
//
//	std::string outfn2{ "output2.txt" };
//	signaturer.SetOutputFileName(outfn2);
//
//	signaturer.Run(); //fill output2
//
//	BOOST_CHECK(compareFiles(outfn, outfn2));
//}
//
//BOOST_AUTO_TEST_CASE(check_difference_for_two_output_files_from_different_input_files)
//{
//	auto& signaturer = Signaturer::GetInstance();
//
//	std::string infn{ "input.txt" };
//	std::string outfn{ "output.txt" };
//	file_generation(infn);
//	
//	signaturer.SetInputFileName(infn);
//	signaturer.SetOutputFileName(outfn);
//
//	signaturer.Run(); //fill output
//	//-----------------------------------------------------
//
//	std::string infn2{ "input2.txt" };
//	std::string outfn2{ "output2.txt" };
//	file_generation(infn2);
//
//	signaturer.SetInputFileName(infn2);
//	signaturer.SetOutputFileName(outfn2);
//
//	signaturer.Run(); //fill output2
//	//-----------------------------------------------------
//
//	BOOST_CHECK(!compareFiles(outfn, outfn2));
//}