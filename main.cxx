// #include <iostream>
// #include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <vector>
#include <string>
#include <fstream>
// #include <openssl/aes.h>
#include "crypto.h"

int main(int argc, char *argv[])
{
	// int error;
	// pthread_t *threadsTab;
	// unsigned *threadArgs;
	std::string password = "password";
	std::string basePath = "./tests";
	// Crypto *crypto;

	bool debug = false;
	bool parallel = false;
	int threads = 1;
	bool encrypt = true;
	bool memory = false;
	int res;

	// std::vector<std::string> filePaths;

	// unsigned totalOperations, step, start;

	// Configuring Parameters
	for (int i = 1; i < argc; i++)
	{

		if (argv[i] == std::string("-password") || argv[i] == std::string("-k"))
		{
			password = argv[i+1];
		}

		else if (argv[i] == std::string("-path") || argv[i] == std::string("-p"))
		{
			basePath = argv[i+1];
		}

		else if (argv[i] == std::string("-threads") || argv[i] == std::string("-t"))
		{
			parallel = true;
			threads = atoi(argv[i+1]);
		}

		else if (argv[i] == std::string("--parallel") || argv[i] == std::string("--p"))
		{
			parallel = true;
		}

		else if (argv[i] == std::string("--verbose") || argv[i] == std::string("--v"))
		{
			debug = true;
		}

		else if (argv[i] == std::string("--encrypt") || argv[i] == std::string("--e"))
		{
			encrypt = true;
		}

		else if (argv[i] == std::string("--decrypt") || argv[i] == std::string("--d"))
		{
			encrypt = false;
		}

		else if (argv[i] == std::string("--memory") || argv[i] == std::string("--m"))
		{
			memory = true;
		}
	}

	res = Crypto::load(debug, parallel, memory, encrypt, basePath, password, threads);
	if (res) return res;

	res = Crypto::run();
	if (res) return res;

	return 0;
}

