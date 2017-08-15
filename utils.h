#ifndef UTILS_H
#define UTILS_H

// Headers
#include <string>
#include <vector>

// Class

class Utils
{
	public:
		static void *find(std::string basePath, std::vector<std::string> *filePaths);
};

#endif