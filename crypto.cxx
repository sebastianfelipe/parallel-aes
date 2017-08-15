#define CRYPTO_CXX
#include "crypto.h"
#include "utils.h"
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <openssl/aes.h>

bool Crypto::encrypt;
bool Crypto::debug;
bool Crypto::parallel;
bool Crypto::memory;
std::string Crypto::basePath;
std::string Crypto::password;
unsigned Crypto::threads;
unsigned Crypto::totalOperations;
unsigned Crypto::step;
std::vector<std::string> Crypto::filePaths;

Crypto::Crypto()
{
};

int Crypto::load(bool debug, bool parallel, bool memory, bool encrypt, std::string basePath, std::string password, unsigned threads)
{
	Crypto::debug = debug;
	Crypto::parallel = parallel;
	Crypto::memory = memory;
	Crypto::encrypt = encrypt;
	Crypto::basePath = basePath;
	Crypto::password = password;
	Crypto::threads = threads;

	if (Crypto::debug) std::cout << "Setting file paths..." << std::endl;

	Utils::find(Crypto::basePath, &Crypto::filePaths);

	Crypto::totalOperations = Crypto::filePaths.size();
	Crypto::threads = (unsigned) fmin(Crypto::threads, Crypto::totalOperations);
	Crypto::step = Crypto::totalOperations/threads;

	if (Crypto::totalOperations == 0)
	{
		printf("%s\n", "ERROR: No files to encrypt/decrypt");
		return 1;
	}

	if (Crypto::debug)
	{
		if (Crypto::totalOperations == 1)
		{
			std::cout << Crypto::totalOperations << " file has been set" << std::endl;
		}
		else
		{
			std::cout << Crypto::totalOperations << " files has been set" << std::endl;
		}
		std::cout << "Base Path: " << Crypto::basePath << std::endl;
		std::cout << "Password: " << Crypto::password << std::endl;
		std::cout << "Parallel: " << (Crypto::parallel ? "True" : "False") << std::endl;
		std::cout << "Threads: " << Crypto::threads << std::endl;
		std::cout << "Process: " << (Crypto::encrypt ? "Encrypt" : "Decrypt") << std::endl;
		std::cout << "Storage: " << (Crypto::memory ? "Memory" : "Files") << std::endl;
	}

	return 0;
};

void *Crypto::encryptAES(std::string input)
{	
	const char ivecstr[AES_BLOCK_SIZE] = "MASTER_KEY\0";
	std::string output, command;
	int bytes_read, bytes_written, num;
	FILE *ifp, *ofp;
  	unsigned char indata[AES_BLOCK_SIZE];
  	unsigned char outdata[AES_BLOCK_SIZE];

  	/* ckey and ivec are the two 128-bits keys necesary to
     en- and recrypt your data.  Note that ckey can be
     192 or 256 bits as well */
  	unsigned char *ckey;

	unsigned char ivec[AES_BLOCK_SIZE];
	

	AES_KEY key;

	output = input + ".tmp";

	command = "";

	ckey = (unsigned char *) Crypto::password.c_str();
	memcpy( ivec , ivecstr, AES_BLOCK_SIZE);

  	/* data structure that contains the key itself */

	/* set the encryption key */
	AES_set_encrypt_key(ckey, 256, &key);
	//  set where on the 128 bit encrypted block to begin encryption

	  num = 0;

	  ifp = fopen(input.c_str(), "rb");

	  if (!Crypto::memory)
	  {
	  	ofp = fopen(output.c_str(), "wb");
	  }

	  while (1) {
	    bytes_read = fread(indata, 1, AES_BLOCK_SIZE, ifp);
	    if (Crypto::encrypt)
	    {
	    	AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num, AES_ENCRYPT);
	    	// printf("%s\n", "After Encrypt!");
	    }
	    else
	    {
	    	AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num, AES_DECRYPT);
	    	// printf("%s\n", "After Decrypt!");
	    }
	    if (!Crypto::memory) bytes_written = fwrite(outdata, 1, bytes_read, ofp);

	    if (bytes_read < AES_BLOCK_SIZE)
	  		break;
	  }

	  fclose(ifp);

	  if (!Crypto::memory)
	  {
	  	fclose(ofp);
	  	command = "mv " + output + " " + input;
		system(command.c_str());
	  }

	return NULL;
}

void *Crypto::runStep(void *start)
{
	unsigned first;
	unsigned next;
	unsigned last;

	first = *((unsigned *) start);
	next = first + Crypto::step - 1;
	last = next;

	if ( (Crypto::totalOperations > Crypto::threads*Crypto::step - 1)
		 && (Crypto::threads*Crypto::step - 1 == last) )
	{
		last = Crypto::totalOperations - 1;
	}

	if (Crypto::debug) printf("This thread %s from file %d to file %d\n", Crypto::encrypt ? "encrypt" : "decrypt", first + 1 , last + 1);
	for (int n = first; n <= last; n++)
	{
		Crypto::encryptAES(Crypto::filePaths.at(n));

		if (Crypto::debug) printf("File %d: %s %s\n", n + 1, Crypto::filePaths.at(n).c_str(), Crypto::encrypt ? "encrypted" : "decrypted");
	}

	return NULL;
}

int Crypto::run()
{
	int error;
	pthread_t *threadsTab;
	unsigned *threadArgs;
	unsigned start;

	
	if (Crypto::debug) std::cout << "Start " << (Crypto::encrypt ? "encrypting" : "decrypting") << " process..." << std::endl;

	// Parallel Version
	if (Crypto::parallel)
	{
		// Threads init
		if (Crypto::debug) std::cout << "Creating Threads..." << std::endl;
		
		threadsTab = (pthread_t *)malloc( threads*sizeof(pthread_t));
		threadArgs = (unsigned *)malloc(sizeof(unsigned)*threads);
		if (Crypto::debug) std::cout << "Threads Initialized" << std::endl;
		for (unsigned i = 0; i < threads; i++)
		{
			threadArgs[i] = i*Crypto::step;
			error = pthread_create( &threadsTab[i], NULL, Crypto::runStep, (void *)&threadArgs[i]);
	 		if (error) return 1;
		}

		for(unsigned i=0; i < threads; i++) pthread_join(threadsTab[i], NULL);
	}

	// Sequential Version
	else
	{
		start = 0;

		for (unsigned i = 0; i < threads; i++)
		{
			start = i*Crypto::step;
			Crypto::runStep(&start);
		}
	}

	if (Crypto::debug) std::cout << "Finish " << (Crypto::encrypt ? "encrypting" : "decrypting") << " process successfully"<< std::endl;

	return 0;
}