#include "djab.h"
#include <algorithm>
#include <assert.h>

namespace djab {

Bundle::Bundle(std::ifstream* inputStream) : inputStream{ inputStream } {
    assert(inputStream->is_open());
    
    char magic;
    read(magic);
    assert(magic == MAGIC_NUMBER);

    read(version);
    read(compressionType);
    read(indexEntrySize);
    //read(dataSectionStart);
    /*inputStream->read((char*)&version, sizeof(version));
    inputStream->read((char*)&compressionType, sizeof(compressionType));
    inputStream->read((char*)&indexEntrySize, sizeof(indexEntrySize));
    inputStream->read((char*)&dataSectionStart, sizeof(dataSectionStart));*/

    for (size_t i = 0; i < indexEntrySize; i++) {
        Index::Entry entry;
        read(entry.key);
        read(entry.offset);
        read(entry.size);
        index.entries.emplace_back(entry);
    }

    dataSectionStart = inputStream->tellg();
}

std::vector<char> Bundle::getBytes(const std::string& key) {
    std::vector<char> result;
    auto it = std::find_if(index.entries.begin(), index.entries.end(), [key](Index::Entry entry) { return key == entry.key; });

	if (it != index.entries.end()) {
		Index::Entry entry = *it;

		result.resize(entry.size);
		inputStream->seekg(dataSectionStart + entry.offset, std::ios::beg);

		inputStream->read(reinterpret_cast<char*>(result.data()), entry.size);
	}

	return result;
}

std::string Bundle::getString(const std::string& path) {
	auto bytes = getBytes(path);
	return std::string(bytes.begin(), bytes.end());
}

}