#include "maze_obstrucle_mapper.h"

MazeObstrucleMapper::MazeObstrucleMapper(const std::string& obstrucleFile) : obstrucleFile(obstrucleFile) {
    std::cout << "Loading File: " << obstrucleFile.c_str() << std::endl;
    loadFile();
}

std::map<std::string, bool> MazeObstrucleMapper::getObstrucle(int x, int y) {
    return obstrucle[std::make_tuple(x, y)];
}

int MazeObstrucleMapper::getSize() {
    return static_cast<int>(std::sqrt(obstrucle.size()));
}

void MazeObstrucleMapper::loadFile() {
    std::ifstream file(obstrucleFile, std::ifstream::binary);
    file >> m_configuration;
    for (const auto& key : m_configuration.getMemberNames()) {
        std::string coordinates = key;
        size_t commaPos = coordinates.find(',');
        int x = std::stoi(coordinates.substr(0, commaPos));
        int y = std::stoi(coordinates.substr(commaPos + 1));
        std::map<std::string, bool> innerMap;
        for (const auto& innerKey : m_configuration[key].getMemberNames()) {
            innerMap[innerKey] = m_configuration[key][innerKey].asBool();
        }
        obstrucle[std::make_tuple(x, y)] = innerMap;
    }
}

