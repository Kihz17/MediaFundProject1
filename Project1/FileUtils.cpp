#include "FileUtils.h"

const std::string ASSETS_DIR = "common\\assets\\audio\\";

std::vector<std::string> GetFilesFromIndex(std::string indexFileName)
{
		std::vector<std::string> result;
		std::stringstream ss;
		ss << SOLUTION_DIR << ASSETS_DIR << indexFileName;

		std::ifstream inputFile(ss.str());
		std::string line;
		while (std::getline(inputFile, line)) {
			result.push_back(line);
		}

		return result;
}
