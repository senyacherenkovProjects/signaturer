#pragma once

#include <cstddef>
#include <string>
#include <string>
#include <vector>
#include <future>

using signature_block_t = std::vector<uint64_t>;

class Signaturer
{
private:
	Signaturer() {}
	Signaturer(const Signaturer&);
	Signaturer& operator=(Signaturer&);
public:
	static Signaturer& GetInstance() {
		static Signaturer  instance;
		return instance;
	}
	
	void ParseArgs(int argc, char* argv[]);
	void Run();
	
	void SetBlockSize(uint64_t bs) { block_size_ = bs; }
	
	//for tests purposes
	void SetInputFileName(const std::string& infn) { input_filename_ = infn; }
	void SetOutputFileName(const std::string& outfn) { output_filename_ = outfn; }

private:

	std::vector<std::string_view> make_chunks(std::string_view entire, std::size_t buff_size, uint16_t nthreads);
	signature_block_t calc_signature(std::string_view chunk);

private:
	std::string input_filename_;
	std::string output_filename_;
	uint64_t block_size_{ 1024 };
};
