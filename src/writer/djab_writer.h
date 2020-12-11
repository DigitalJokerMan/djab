#pragma once
#include <djab.h>
#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

namespace djab {

class BundleWriter {
private:
    struct FileEntry {
        std::string sourcePath;
        std::string bundleKey;
    };
    std::vector<FileEntry> filesQueue;
    std::ofstream outputStream;
    Index index;
    unsigned char version{ 2 }, compressionType{ 0 };

public:
    BundleWriter(const std::string& outputPath) : outputStream(outputPath) {
        assert(outputStream.is_open());
    }

    ~BundleWriter() {
        outputStream.close();
    }

    void addFile(const std::string& sourcePath, const std::string& bundleKey) {
        filesQueue.emplace_back(FileEntry{ sourcePath, bundleKey });
    }

    void flush(const std::string& dataTempPath = "data.tmp") {
        write(MAGIC_NUMBER);
        write(version);
        write(compressionType);
        auto indexSize{ filesQueue.size() };
        write(indexSize);

        std::ofstream dataTempFile(dataTempPath);
        assert(dataTempFile.is_open());

        for (size_t i = 0; i < filesQueue.size(); i++) {
            std::ifstream file(filesQueue[i].sourcePath);
            assert(file.is_open());

            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            // Create a buffer and read the source file into it
		    //char* buffer = new char[size];
		    //file.read(buffer, size);

            dataTempFile.seekp(0, std::ios::end);
            size_t offset = dataTempFile.tellp();

            dataTempFile << file.rdbuf();

            write(filesQueue[i].bundleKey);
            write(offset);
            write(size);

            file.close();
        }

        dataTempFile.close();

        dataTempFile.seekp(0, std::ios::beg);

        std::ifstream dataInput(dataTempPath);
        /*dataInput.seekg(0, std::ios::end);
        size_t dataSize = dataInput.tellg();
        dataInput.seekg(0, std::ios::beg);

        char* buffer = new char[dataSize];
	    dataInput.read(buffer, dataSize);

        outputStream.write(buffer, dataSize);*/

        outputStream << dataInput.rdbuf();
        //outputStream.write(dataTempFile.rdbuf());
        //outputStream << dataTempFile;
    }

private:
    template <typename T>
    void write(T& value);
};

template<typename T>
void BundleWriter::write(T& value) {
    if constexpr (std::is_base_of<std::string, T>::value) {
        const char* cString = value.c_str();
	    unsigned long size = value.size();

	    write(size);
	    outputStream.write(cString, size);
    } else {
	    outputStream.write((const char*)&value, sizeof(value));
    }
}

/*template<>
inline void BundleWriter::write(std::string& value) {
	const char* cString = value.c_str();
	unsigned long size = value.size();

	write(size);
	outputStream.write(cString, size);
}*/


}