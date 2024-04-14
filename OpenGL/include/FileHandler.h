#include <fstream>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

/*
	FileHandler.h - read vertices && indices from file. Maybe other things too in the future
*/
template<typename T>
std::vector<T> read_content_from_file(std::string path) {
	std::vector<T> toReturn;
	
	std::fstream* file = new std::fstream;
	file->open(path, std::ios_base::in);

	if (file->bad()) {
		std::cout << "Can't read file ";
		return {};
	}
	std::string line;
	while (std::getline(*file, line)) {
		std::stringstream s(line);
		
		T value;
		while (s >> value) 	toReturn.push_back(value);
	}
	file->close();
	return toReturn;
}