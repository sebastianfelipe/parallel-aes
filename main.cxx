#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <openssl/aes.h>

// #include <ctime>

// DEBUG
bool DEBUG = false;
bool PARALLEL = false;
bool MEMORY = false;

// FILE PATHS
std::vector<std::string> FILE_PATHS;

// ENCRYPT
bool ENCRYPT = true;

// BASE_PATH
std::string BASE_PATH = "./tests/bunch-1";

// PASSWORD
std::string PASSWORD = "passpass";

// THREADS
unsigned THREADS = 1;

// TOTAL OPERATIONS
unsigned TOTAL_OPERATIONS;

// STEP
unsigned STEP;

const char ivecstr[AES_BLOCK_SIZE] = "MASTER_KEY\0";

void *encrypt_file(std::string input, std::string output, std::string password) {
	int bytes_read, bytes_written;
  	unsigned char indata[AES_BLOCK_SIZE];
  	unsigned char outdata[AES_BLOCK_SIZE];

  	/* ckey and ivec are the two 128-bits keys necesary to
     en- and recrypt your data.  Note that ckey can be
     192 or 256 bits as well */
  	unsigned char *ckey = (unsigned char *) password.c_str();
  	// unsigned char *ivec = (unsigned char *) password.c_str();

	unsigned char ivec[AES_BLOCK_SIZE];
	memcpy( ivec , ivecstr, AES_BLOCK_SIZE);

  	/* data structure that contains the key itself */
	  AES_KEY key;

	  /* set the encryption key */
	  AES_set_encrypt_key(ckey, 256, &key);

	  /* set where on the 128 bit encrypted block to begin encryption*/
	  int num = 0;
	  FILE *ifp = fopen(input.c_str(), "rb");
	  FILE *ofp = fopen(output.c_str(), "wb");

	  while (1) {
	    bytes_read = fread(indata, 1, AES_BLOCK_SIZE, ifp);
	    if (ENCRYPT)
	    {
	    	AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num,
	           AES_ENCRYPT);
	    }
	    else
	    {
	    	AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num,
	           AES_DECRYPT);
	    }
	    if (!MEMORY)
	    {
	    	bytes_written = fwrite(outdata, 1, bytes_read, ofp);
	    }

	    if (bytes_read < AES_BLOCK_SIZE)
	  		break;
	  }

	  fclose(ifp);
	  fclose(ofp);


	// AES_KEY enc_key, dec_key;

	// AES_set_encrypt_key((const unsigned char *) password.c_str(), 128, &enc_key);

	// AES_set_decrypt_key((const unsigned char *) password.c_str(), 128, &dec_key);

	return NULL;
}

void *encrypt_step(std::string path) {
	std::string command;
	std::string tmp;

	tmp = path + ".tmp";

	command = "openssl aes-256-cbc -a -salt -in";
	command += " " + path;
	command += " -out";
	command += " " + tmp;
	command += " -k " + PASSWORD;
	

	// system(command.c_str());
	encrypt_file(path, tmp, PASSWORD);

	// Replace the original file by the temporal file
	if (!MEMORY)
	{
		command = "mv " + tmp + " " + path;
		system(command.c_str());
	}
	else
	{
		command = "rm " + tmp;
		system(command.c_str());
	}

	return NULL;
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

	// system(command.c_str());
	encrypt_file(path, tmp, PASSWORD);

	// Replace the original file by the temporal file
	if (!MEMORY)
	{
		command = "mv " + tmp + " " + path;
		system(command.c_str());
	}
	else
	{
		command = "rm " + tmp;
		system(command.c_str());
	}

	return NULL;
}

void *aes(void *start) {
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
		if (DEBUG) printf("This thread encrypt from file %d to file %d\n", first + 1 , last + 1);
		for (int n = first; n <= last; n++)
		{
			encrypt_step(FILE_PATHS.at(n));
			if (DEBUG) printf("File %d encrypted\n", n + 1);
		}
	}
	else
	{
		if (DEBUG) printf("This thread decrypt from file %d to file %d\n", first + 1, last + 1);
		for (int n = first; n <= last; n++)
		{
			decrypt_step(FILE_PATHS.at(n));
			if (DEBUG) printf("File %d decrypted\n", n + 1);
		}
	}

	return NULL;

	// Remove the original file

	// Erase the .tmp extension of the new file
}

void find(std::string base_path) {
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

	// Configuring Parameters
	for (int i = 1; i < argc; i++)
	{

		if (argv[i] == std::string("-password") || argv[i] == std::string("-k"))
		{
			PASSWORD = argv[i+1];
		}

		else if (argv[i] == std::string("-path") || argv[i] == std::string("-p"))
		{
			BASE_PATH = argv[i+1];
		}

		else if (argv[i] == std::string("-threads") || argv[i] == std::string("-t"))
		{
			PARALLEL = true;
			THREADS = atoi(argv[i+1]);
		}

		else if (argv[i] == std::string("--parallel") || argv[i] == std::string("--p"))
		{
			PARALLEL = true;
		}

		else if (argv[i] == std::string("--verbose") || argv[i] == std::string("--v"))
		{
			DEBUG = true;
		}

		else if (argv[i] == std::string("--encrypt") || argv[i] == std::string("--e"))
		{
			ENCRYPT = true;
		}

		else if (argv[i] == std::string("--decrypt") || argv[i] == std::string("--d"))
		{
			ENCRYPT = false;
		}

		else if (argv[i] == std::string("--memory") || argv[i] == std::string("--m"))
		{
			MEMORY = true;
		}
	}


	// Generate random vectors values (matrix representation)
	// Initialize vector A

	if (DEBUG) std::cout << "Setting file paths..." << std::endl;

	find(BASE_PATH);

	TOTAL_OPERATIONS = FILE_PATHS.size();
	THREADS = (unsigned) fmin(THREADS, TOTAL_OPERATIONS);
	STEP = TOTAL_OPERATIONS/THREADS;

	if (TOTAL_OPERATIONS == 0)
	{
		std::cout << "ERROR: No files to encrypt/decrypt";
		return 1;
	}

	if (DEBUG)
	{
		if (TOTAL_OPERATIONS == 1)
		{
			std::cout << TOTAL_OPERATIONS << " file has been set" << std::endl;
		}
		else
		{
			std::cout << TOTAL_OPERATIONS << " files has been set" << std::endl;
		}
		std::cout << "BASE_PATH: " << BASE_PATH << std::endl;
		std::cout << "PASSWORD: " << PASSWORD << std::endl;
		std::cout << "PARALLEL: " << (PARALLEL ? "true" : "false") << std::endl;
		std::cout << "THREADS: " << THREADS << std::endl;
		

	}

	// Threads init

	if (DEBUG) std::cout << "Start " << (ENCRYPT ? "encrypting" : "decrypting") << " process..." << std::endl;

	// encrypt_step("/Users/tiantg/Projects/parallel-aes/tests/lorem-ipsum/data.txt");
	// decrypt_step("/Users/tiantg/Projects/parallel-aes/tests/lorem-ipsum/data.txt");

	// initialTime = std::time(NULL);

	// Parallel Version
	if (PARALLEL)
	{
		if (DEBUG) std::cout << "Creating Threads..." << std::endl;
		
		threads_tab = (pthread_t *)malloc( THREADS*sizeof(pthread_t));
		thread_args = (unsigned *)malloc(sizeof(unsigned)*THREADS);
		// if (DEBUG) std::cout << "Threads Initialized" << std::endl;
		if (DEBUG) std::cout << "Threads Initialized" << std::endl;
		for (unsigned i = 0; i < THREADS; i++)
		{
			thread_args[i] = i*STEP;
			error = pthread_create( &threads_tab[i], NULL, aes, (void *)&thread_args[i]);
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
			aes(&start);
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

	if (DEBUG) std::cout << "Finish " << (ENCRYPT ? "encrypting" : "decrypting") << " process successfully"<< std::endl;
	// std::cout <<  "Timing: " << timeElapsed << "[s]" << std::endl;

	return 0;
}