#ifndef CRYPTO_H
#define CRYPTO_H

// Headers
#include <vector>
#include <string>
// #include <ctime>

// #include <openssl/aes.h>

// Class

class Crypto
{
	private:
		static bool encrypt;
		static bool debug;
		static bool parallel;
		static bool memory;
		static std::string basePath;
		static std::string password;
		static unsigned threads;
		static unsigned totalOperations;
		static unsigned step;
		static std::vector<std::string> filePaths;

	public:
		Crypto();
		// Crypto(bool debug, bool parallel, bool memory, bool encrypt, std::string basePath, std::string password, unsigned threads);
		static int load(bool debug, bool parallel, bool memory, bool encrypt, std::string basePath, std::string password, unsigned threads);
		static void *encryptAES(std::string input);
		static void *runStep(void *start);
		static int run();
		// void *setDebug(bool debug);
		// void *setParallel(bool parallel);
		// void *setMemory(bool memory);
		// void *setEncrypt(bool encrypt);
};

#endif