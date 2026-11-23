#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <tuple>
#include <cassert>
#include <json/value.h>
#include <json/json.h>

class MazeObstrucleMapper {
private:
    std::string obstrucleFile;
    std::map<std::tuple<int, int>, std::map<std::string, bool>> obstrucle;
    void loadFile();
    Json::Value m_configuration;

public:
    MazeObstrucleMapper(const std::string& obstrucleFile);
    std::map<std::string, bool> getObstrucle(int x, int y);
    int getSize();
};