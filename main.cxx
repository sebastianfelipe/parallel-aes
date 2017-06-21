#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <vector>
#include <string>
#include <fstream>
// #include <ctime>

// DEBUG
bool DEBUG = false;
bool PARALLEL = false;

// FILE PATHS
std::vector<std::string> FILE_PATHS;

// ENCRYPT
bool ENCRYPT = true;

// PASSWORD
std::string PASSWORD = "passpass";

// THREADS
unsigned THREADS = 1;

// TOTAL OPERATIONS
unsigned TOTAL_OPERATIONS;

// STEP
unsigned STEP;

void *encrypt_step(std::string path) {
	std::string command;
	std::string tmp;

	tmp = path + ".tmp";

	command = "openssl aes-256-cbc -a -salt -in";
	command += " " + path;
	command += " -out";
	command += " " + tmp;
	command += " -k " + PASSWORD;

	system(command.c_str());

	// Replace the original file by the temporal file
	command = "mv " + tmp + " " + path;
	system(command.c_str());
}

void *decrypt_step(std::string path) {
	std::string command;
	std::string tmp;

	tmp = path + ".tmp";

	command = "openssl aes-256-cbc -d -a -in ";
	command += " " + path;
	command += " -out";
	command += " " + tmp;
	command += " -k " + PASSWORD;

	system(command.c_str());

	// Replace the original file by the temporal file
	command = "mv " + tmp + " " + path;
	system(command.c_str());
}

void *rsa(void *start) {
	unsigned first;
	unsigned next;
	unsigned last;

	first = *((unsigned *) start);
	next = first + STEP - 1;
	last = next;

	if ( (TOTAL_OPERATIONS > THREADS*STEP - 1)
		 && (THREADS*STEP - 1 == last) )
	{
		last = TOTAL_OPERATIONS - 1;
	}

	if (ENCRYPT)
	{
		for (int n = first; n <= last; n++)
		{
			encrypt_step(FILE_PATHS.at(n));
		}
	}
	else
	{
		for (int n = first; n <= last; n++)
		{
			decrypt_step(FILE_PATHS.at(n));
		}
	}

	// Remove the original file

	// Erase the .tmp extension of the new file
}

void *find(std::string base_path) {
	std::string command;
	std::string tmp("files.tmp");

	// Find every file into the base_path (depth search) and save every path in a temporal file
	command = "find";
	command += " " + base_path;
	command += " -type f";
	command += " > " + tmp;
	system(command.c_str());

	// Every path has to be saved using the FILE_PATHS constant;
	std::string line;
	std::ifstream file (tmp);

	if (file.is_open())
	{
		while ( std::getline (file,line) )
		{
			FILE_PATHS.push_back(line);
		}
	}
	else
	{
		std::cout << "Do something" << std::endl;
	}

	// Finally, the temporal file is deleted as follow
	command = "rm -rf";
	command += " " + tmp;
	system(command.c_str());
}

int main(int argc, char *argv[])
{
	int error;
	pthread_t *threads_tab;
	unsigned *thread_args;
	// clock_t timeElapsed;

	// std::time_t initialTime, finishTime;

	// // Configuring Parameters
	// for (int i = 1; i < argc; i++)
	// {
	// 	if (argv[i] == std::string("-n"))
	// 	{
	// 		N = atoi(argv[i+1]);
	// 	}

	// 	else if (argv[i] == std::string("-m"))
	// 	{
	// 		M = atoi(argv[i+1]);
	// 	}

	// 	else if (argv[i] == std::string("-p"))
	// 	{
	// 		P = atoi(argv[i+1]);
	// 	}

	// 	else if (argv[i] == std::string("-t"))
	// 	{
	// 		PARALLEL = true;
	// 		THREADS = atoi(argv[i+1]);
	// 	}

	// 	else if (argv[i] == std::string("--parallel") || argv[i] == std::string("--p"))
	// 	{
	// 		PARALLEL = true;
	// 	}

	// 	else if (argv[i] == std::string("--verbose") || argv[i] == std::string("--v"))
	// 	{
	// 		DEBUG = true;
	// 	}
	// }



	// Generate random vectors values (matrix representation)
	// Initialize vector A

	std::cout << "Setting file paths..." << std::endl;

	find("/Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum");

	TOTAL_OPERATIONS = FILE_PATHS.size();
	THREADS = (unsigned) fmin(THREADS, TOTAL_OPERATIONS);
	STEP = TOTAL_OPERATIONS/THREADS;

	if (TOTAL_OPERATIONS == 0)
	{
		std::cout << "ERROR: No files to encrypt/decrypt";
		return 1;
	}
	else if (TOTAL_OPERATIONS == 1)
	{
		std::cout << TOTAL_OPERATIONS << " file has been set" << std::endl;
	}
	else
	{
		std::cout << TOTAL_OPERATIONS << " files has been set" << std::endl;
	}

	std::cout << std::endl;

	// Threads init

	printf("Start encrypting...\n");

	// encrypt_step("/Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt");
	// decrypt_step("/Users/tiantg/Projects/parallel-rsa/tests/lorem-ipsum/data.txt");

	// initialTime = std::time(NULL);

	// Parallel Version
	if (PARALLEL)
	{
		std::cout << "Creating Threads..." << std::endl;
		threads_tab = (pthread_t *)malloc( THREADS*sizeof(pthread_t));
		thread_args = (unsigned *)malloc(sizeof(unsigned)*THREADS);
		// if (DEBUG) std::cout << "Threads Initialized" << std::endl;
		std::cout << "Threads Initialized" << std::endl;
		for (unsigned i = 0; i < THREADS; i++)
		{
			thread_args[i] = i*STEP;
			error = pthread_create( &threads_tab[i], NULL, rsa, (void *)&thread_args[i]);
	 		if (error) return 1;
		}

		for(unsigned i=0; i < THREADS; i++) pthread_join(threads_tab[i], NULL);
	}

	// Sequential Version
	else
	{
		unsigned start = 0;

		for (unsigned i = 0; i < THREADS; i++)
		{
			start = i*STEP;
			rsa(&start);
		}
	}

	// finishTime = std::time(NULL);
	// timeElapsed = finishTime - initialTime;

	// Result
	// if (DEBUG)
	// {
	// 	std::cout << "Vector: C" << "(" << N << "x" << P << ")";
	// 	for (int i = 0; i < N*P; i++)
	// 	{		
	// 		if (i%P == 0)
	// 		{
	// 			std::cout << std::endl;
	// 		}

	// 		std::cout << C.at(i) << "\t";
	// 	}

	// 	std::cout << std::endl << std::endl;
	// }

	std::cout << "Finish encrypting" << std::endl;
	// std::cout <<  "Timing: " << timeElapsed << "[s]" << std::endl;

	return 0;
}