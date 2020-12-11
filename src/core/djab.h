#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace djab {

const unsigned char MAGIC_NUMBER = 17;

struct Index {
    struct Entry {
        std::string key;
        unsigned long long offset;
        unsigned long long size;
    };

    std::vector<Entry> entries;
};

class Bundle {
private:
    std::ifstream* inputStream;
    Index index;

	template<typename T>
	T read();

	template<typename T>
	void read(T& value);

public:
    unsigned char version, compressionType;
    size_t indexEntrySize, dataSectionStart;
    
    Bundle(std::ifstream* inputStream);

    // I might be wrong about this, but I feel like using vectors is a little less efficient than
    // using a char* or something, but I'm 99% sure that using a vector is the right choice because
    // technically I'm trying to use a "dynamically-sized" array.
    std::vector<char> getBytes(const std::string& key);
	std::string getString(const std::string& path);
};

template<typename T>
T Bundle::read() {
	T value;
	inputStream->read((char*)&value, sizeof(value));
	return value;
}

template<typename T>
void Bundle::read(T& value) {
	inputStream->read((char*)&value, sizeof(value));
}

template<>
inline void Bundle::read<std::string>(std::string& value) {
	unsigned long size = read<size_t>();
	for (size_t i = 0; i < size; i++) value += read<char>();
}

}