#include "MazeObstacleMapper.h"

MazeObstacleMapper::MazeObstacleMapper(const std::string& obstacleFile) : obstacleFile(obstacleFile) {
    std::cout << "Loading File: " << obstacleFile.c_str() << std::endl;
    obstacle = loadFile();
}

uint8_t MazeObstacleMapper::operator[](uint16_t coordinates) {
    return obstacle[coordinates];
}

uint8_t MazeObstacleMapper::getObstacle(uint16_t coordinates) {
    return obstacle[coordinates];
}

uint16_t MazeObstacleMapper::length() {
    return static_cast<uint16_t>(std::sqrt(obstacle.size()));
}

std::map<uint16_t, uint8_t> MazeObstacleMapper::loadFile() {
    std::ifstream file(obstacleFile, std::ifstream::binary);
    file >> m_configuration;
    std::map<uint16_t, uint8_t> result;
    for (const auto& key : m_configuration.getMemberNames()) {
        std::string coordinates = key;
        result[processKey(key)] = processValue(m_configuration[key]);
    }
    return result;
}

uint16_t MazeObstacleMapper::processKey(const std::string& key) {
    size_t commaPos = key.find(',');
    uint8_t x = std::stoi(key.substr(0, commaPos));
    uint8_t y = std::stoi(key.substr(commaPos + 1));
    uint16_t value = 0;
    value |= x;
    value = value << factor;
    value |= y;
    return value;
}

uint8_t MazeObstacleMapper::processValue(const Json::Value& value) {
    uint8_t binValue = 0;
    std::string array[] = PROCESSING_ORDER;
    for (int index = 0; index < 4; ++index) {
        binValue |= value[array[index]].asInt();
        if (index != 3) {
            binValue = binValue << 1;
        }
    }
    return binValue;
}
