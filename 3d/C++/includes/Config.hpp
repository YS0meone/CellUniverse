#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include "Sphere.cpp"

std::map<std::string, std::string> parseConfig(const std::string& filename) {
    auto ltrim = [](const std::string& s) -> std::string {
        size_t start = s.find_first_not_of(" \t");
        return (start == std::string::npos) ? "" : s.substr(start);
    }; //used to remove first space

    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line)) {
        line = ltrim(line);
        if (line.empty() || line[0] == '#') continue;

        std::istringstream is_line(line);
        std::string key;
        if (getline(is_line, key, ':')) {
            std::string value;
            if (getline(is_line, value)) {
                config[key] = ltrim(value);
            }
        }
    }
    return config;
}

std::unique_ptr<BaseConfig> loadConfig(const std::string& path) { //used unique ptr to prevent possible memory leak
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return nullptr;
    }
    auto config = parseConfig(path);
	std::unique_ptr<BaseConfig> basecf = std::make_unique<BaseConfig>();

        std::string cellTypeStr = config["cellType"];
        if (cellTypeStr == "sphere") {
            basecf->CellType = 's';
        }
        else if (cellTypeStr == "bacilli") {
            basecf->CellType = 'b';
        }
        else {
            std::cerr << "Invalid cell type: " << cellTypeStr << std::endl;
            return nullptr;
        }
        return basecf;
}

#endif
