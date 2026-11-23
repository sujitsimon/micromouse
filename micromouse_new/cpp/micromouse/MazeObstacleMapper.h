#ifndef MAZE_OBSTACLE_MAPPER_H
#define MAZE_OBSTACLE_MAPPER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <json/value.h>
#include <json/json.h>
#include "MyMacros.h"

class MazeObstacleMapper {
private:
    std::string obstacleFile;
    uint8_t factor = FACTOR;
    std::map<uint16_t, uint8_t> obstacle;
    std::map<uint16_t, uint8_t> loadFile();
    uint16_t processKey(const std::string& key);
    uint8_t processValue(const Json::Value& value);
    Json::Value m_configuration;

public:
    MazeObstacleMapper(const std::string& obstacleFile);
    uint8_t operator[](uint16_t coordinates);
    uint8_t getObstacle(uint16_t coordinates);
    uint16_t length();
};

#endif   //MAZE_OBSTACLE_MAPPER_H