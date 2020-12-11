#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "djab_writer.h"

void writeBundle() {
    djab::BundleWriter writer("test_bundle.djab");

    const std::string basePath{ "./assets" };

    for (const auto& fileEntry : std::filesystem::recursive_directory_iterator(basePath)) {
        if (fileEntry.is_directory()) continue;
		std::string srcPath = fileEntry.path().string();
		std::string destPath = fileEntry.path().string();

		std::replace(destPath.begin(), destPath.end(), '\\', '/');
		destPath = destPath.erase(0, basePath.length() + 1);

		writer.addFile(srcPath, destPath);
    }

    writer.flush();
}

void readData() {
    std::ifstream bundleFile("test_bundle.djab", std::ios::binary);
    djab::Bundle bundle(&bundleFile);
    std::cout << bundle.getString("scene.json") << "\n";

    //djab::Bundle bundle(reinterpret_cast<std::istream>(bundleFile));
}

int main(int argc, char** argv) {
    //writeBundle();
    readData();
    return 0;
}