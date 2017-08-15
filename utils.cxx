#define UTILS_CXX
#include "utils.h"

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

void *Utils::find(std::string basePath, std::vector<std::string> *filePaths)
{	
	
	std::string command;
	std::string tmp("files.tmp");

	// Find every file into the basePath (depth search) and save every path in a temporal file
	command = "find";
	command += " " + basePath;
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
			filePaths->push_back(line);
		}
	}
	else
	{
		printf("%s\n", "ERROR: The temporal file cannot be opened.");
	}

	// Finally, the temporal file is deleted as follow
	command = "rm -rf";
	command += " " + tmp;
	system(command.c_str());

	return NULL;
}