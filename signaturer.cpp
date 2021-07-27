#include "signaturer.h"

#include <thread>
#include <fstream>
#include <cctype>
#include <iostream>

#include <boost/functional/hash.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/lexical_cast.hpp>

namespace bip = boost::interprocess;

void Signaturer::ParseArgs(int argc, char* argv[])
{
	if (argc != 3 && argc != 4)
	{
		throw std::runtime_error("wrong number of arguments");
	}

	input_filename_ = argv[1];
	output_filename_ = argv[2];

	if (argc == 4)
	{
		try {
			block_size_ = boost::lexical_cast<uint64_t>(argv[3]); //throws exception in case of bad cast
		}
		catch (boost::bad_lexical_cast& e) {
			std::cerr << e.what();
			exit(1);
		}

		if (block_size_ == 0) {
			throw std::runtime_error("Math error: Attempted to divide by Zero\n");
		}
	}
}

void Signaturer::Run()
{
	try {
		bip::file_mapping mapping(input_filename_.c_str(), bip::read_only);
		bip::mapped_region mapped_rgn(mapping, bip::read_only);

		char const* const mmaped_data = static_cast<char*>(mapped_rgn.get_address());
		std::size_t const mmap_size = mapped_rgn.get_size();

		auto n_threads = std::thread::hardware_concurrency();
		if (n_threads == 0)
			n_threads = 2; //assume that core has HTT 

		auto chunks = make_chunks(mmaped_data, mmap_size, n_threads);

		std::vector<std::future<signature_block_t>> future_signatures;
		future_signatures.reserve(n_threads);

		for (std::string_view chunk : chunks) {
			future_signatures.push_back(std::async(std::launch::async, &Signaturer::calc_signature, this, chunk));
		}
		auto file = std::ofstream{ output_filename_.data() };
		if (!file.is_open())
		{
			throw std::runtime_error("cannot open the output file");
		}

		for (auto& fut_vector : future_signatures) {
			auto signatures = fut_vector.get();
			for (auto& value : signatures) {
				file << value;
			}
		}
	}
	catch (bip::interprocess_exception& e)
	{
		std::cerr << e.what();
		exit(1);
	}
	
}

std::vector<std::string_view> Signaturer::make_chunks(std::string_view entire, std::size_t buff_size, uint16_t nthreads)
{
	std::vector<std::string_view> result;
	result.resize(nthreads);
	
	auto n_blocks = buff_size / block_size_ + 1;
	auto blocks_per_thread = n_blocks / nthreads;
	uint64_t bytes_per_thread = blocks_per_thread * block_size_;

	auto start = entire.data();
	auto current = entire.data();
	auto end = entire.data() + buff_size;

	while (buff_size >= 2 * bytes_per_thread)
	{
		current += bytes_per_thread;
		result.emplace_back(start, static_cast<std::size_t>(current - start));
		start = current + 1;
		buff_size -= bytes_per_thread;
	}
	result.emplace_back(start, static_cast<std::size_t>(end - start));


	return result;
}

signature_block_t Signaturer::calc_signature(std::string_view chunk)
{
	size_t seed = 0;
	signature_block_t result;

	auto start = chunk.data();
	auto current = chunk.data();
	auto internal_size = chunk.size();

	while (internal_size >= 2 * block_size_) {
		for (const auto& ch : std::string_view(start, block_size_))
			boost::hash_combine(seed, ch);

		result.push_back(seed);

		start += block_size_ + 1;

		internal_size -= block_size_;
		seed = 0;
	}

	for (const auto& ch : std::string_view(start, internal_size))
		boost::hash_combine(seed, ch);
	result.push_back(seed);
	
	return result;
}
