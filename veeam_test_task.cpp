// veeam_test_task.cpp : Defines the entry point for the application.
//

#include "signaturer.h"
#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>


int main(int argc, char* argv[])
{
	try {
		auto& instance = Signaturer::GetInstance();
		instance.ParseArgs(argc, argv);
		instance.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what();
	}
	catch (...) {}
	
	return 0;
}
