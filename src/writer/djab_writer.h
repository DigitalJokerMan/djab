#pragma once
#include <djab.h>
#include <assert.h>
#include <fstream>
#include <filesystem>
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
            // Open the file.
            std::ifstream file(filesQueue[i].sourcePath);
            assert(file.is_open());

            // Get file size.
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            // Get offset.
            dataTempFile.seekp(0, std::ios::end);
            size_t offset = dataTempFile.tellp();

            // Write the file into data.
            dataTempFile << file.rdbuf();

            write(filesQueue[i].bundleKey);
            write(offset);
            write(size);

            file.close();
        }

        dataTempFile.close();

        // Write the data into the main file.
        std::ifstream dataInput(dataTempPath);

        outputStream << dataInput.rdbuf();
        dataInput.close();
        
        // Delete the data file.
        std::filesystem::remove(dataTempPath); // TODO: Handle errors from this.
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