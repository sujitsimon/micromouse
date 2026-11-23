#include "maze_memory.h"

MazeMemory *MazeMemory::m_mazememory = nullptr;

MazeMemory::MazeMemory(uint8_t max_x, uint8_t max_y, bool stub, Logger *loggerInstance) : m_max_x(max_x), m_max_y(max_y), stub(stub), m_logger(loggerInstance)
{
    if (stub)
    {
        updateSavedObstrucleMemory();
    }
}
void MazeMemory::addVisited(uint8_t x, uint8_t y)
{
    visited[Mouse_Utils::encode_coordinates(x, y)] = true;
}

bool MazeMemory::isVisited(uint8_t x, uint8_t y)
{
    return visited.find(Mouse_Utils::encode_coordinates(x, y)) != visited.end();
}

uint8_t MazeMemory::getNumberOfPathsAvailable() const
{
    return path_available;
}

bool MazeMemory::isFourInterconnected()
{
    int missed_path = 0;
    for (const auto &each_node : destination_mapper)
    {
        uint8_t x, y;
        Mouse_Utils::decode_coordinates(each_node, x, y);
        for (const auto &each_path : getPossibleNodes(x, y))
        {
            if (std::find(destination_mapper.begin(), destination_mapper.end(), each_path) == destination_mapper.end())
            {
                missed_path++;
            }
        }
    }
    return missed_path == 1;
}

bool MazeMemory::isDestinationFound() {
    return !(destination_point == NO_PATH);
}

uint16_t MazeMemory::findExactDestination(uint16_t entryCoordinate, uint8_t max_x, uint8_t max_y) {
    //uint8_t current_grid = memory[Mouse_Utils::encode_coordinates(current_x, current_y)];
    uint8_t current_grid = memory[entryCoordinate];
    auto grid_info = Mouse_Utils::obstacleDecoder(current_grid);
    if ((grid_info["left"] == false) || (grid_info["top"] == false)) {
        return destination_mapper[1];
    } else if (grid_info["right"] == false) {
        return destination_mapper[3];
    } else {
        uint8_t x;
        uint8_t y;
        Mouse_Utils::decode_coordinates(entryCoordinate, x, y);
        if ((x == max_x) && (y == max_y)){
            return destination_mapper[1];
        }
    }
    return destination_mapper[3];
}

void MazeMemory::checkIsValidDestination()
{
    uint8_t min_x = m_max_x, min_y = m_max_y;
    uint8_t max_x = 0, max_y = 0;

    for (const auto &each : destination_mapper)
    {
        uint8_t x = 0, y = 0;
        Mouse_Utils::decode_coordinates(each, x, y);
        min_x = min(min_x, x);
        min_y = min(min_y, y);
        max_x = max(max_x, x);
        max_y = max(max_y, y);
    }

    if ((max_x - min_x == 1) && (max_y - min_y == 1) && isFourInterconnected())
    {
        m_logger->print("Destination Found: (");
        m_logger->print(int(min_x));
        m_logger->print(", ");
        m_logger->print(int(min_y));
        m_logger->print(") to (");
        m_logger->print(int(max_x));
        m_logger->print(", ");
        m_logger->print(int(max_y));
        m_logger->println(")");
        destination["min_x"] = min_x;
        destination["min_y"] = min_y;
        destination["max_x"] = max_x;
        destination["max_y"] = max_y;
        destination_point = findExactDestination(destination_mapper[0], max_x, max_y);
    }
    destination_mapper.clear();
    may_be_destination = false;
}

bool MazeMemory::isDeadEnd(uint8_t current_x, uint8_t current_y) {
    uint8_t current_grid = memory[Mouse_Utils::encode_coordinates(current_x, current_y)];
    uint8_t counter = 0;
    while(current_grid) {
        counter += current_grid & 1;
        current_grid >>= 1;
    }
    return (counter == 3);
}


uint16_t MazeMemory::getDestination()
{
    return destination_point;
}

void MazeMemory::updateDestination(uint8_t current_x, uint8_t current_y)
{
    if (may_be_destination)
    {
        if (path_available == 1 || (destination_mapper.size() == 3 && path_available == 0))
        {
            destination_mapper.push_back(Mouse_Utils::encode_coordinates(current_x, current_y));
        }
        else
        {
            destination_mapper.clear();
            may_be_destination = false;
        }
    }
    if (destination_mapper.size() == 4)
    {
        checkIsValidDestination();
    }
    if (path_available == 2)
    {
        may_be_destination = true;
        destination_mapper.push_back(Mouse_Utils::encode_coordinates(current_x, current_y));
    }
}

std::vector<uint16_t> MazeMemory::getPossibleNodes(uint8_t current_x, uint8_t current_y)
{
    if (!isVisited(current_x, current_y))
        return {};

    uint8_t current_grid = memory[Mouse_Utils::encode_coordinates(current_x, current_y)];
    auto grid_info = Mouse_Utils::obstacleDecoder(current_grid);

    std::vector<uint16_t> possible_nodes;
    if (current_x - 1 >= 0 && !grid_info["left"])
    {
        possible_nodes.push_back(Mouse_Utils::encode_coordinates(current_x - 1, current_y));
    }
    if (current_y - 1 >= 0 && !grid_info["top"])
    {
        possible_nodes.push_back(Mouse_Utils::encode_coordinates(current_x, current_y - 1));
    }
    if (current_y + 1 <= m_max_y && !grid_info["bottom"])
    {
        possible_nodes.push_back(Mouse_Utils::encode_coordinates(current_x, current_y + 1));
    }
    if (current_x + 1 <= m_max_x && !grid_info["right"])
    {
        possible_nodes.push_back(Mouse_Utils::encode_coordinates(current_x + 1, current_y));
    }

    return possible_nodes;
}

uint16_t MazeMemory::getNextPathLearning(uint8_t current_x, uint8_t current_y)
{
    path_available = 0;
    uint16_t next_path = NO_PATH;

    std::vector<uint16_t> possible_nodes = getPossibleNodes(current_x, current_y);
    for (const auto &node : possible_nodes)
    {
        uint8_t x = 0, y = 0;
        Mouse_Utils::decode_coordinates(node, x, y);
        if (!isVisited(x, y))
        {
            path_available++;
            next_path = node;
        }
    }

    if (!(destination_point == -1))
    {
        updateDestination(current_x, current_y);
    }

    return next_path;
}

bool MazeMemory::isBacktrackingRequired(uint8_t current_x, uint8_t current_y)
{
    path_available = 0;
    std::vector<uint16_t> possible_nodes = getPossibleNodes(current_x, current_y);
    for (const auto &node : possible_nodes)
    {
        uint8_t x = 0, y = 0;
        Mouse_Utils::decode_coordinates(node, x, y);
        if (!isVisited(x, y))
        {
            path_available++;
        }
    }
    return (path_available > 0);
}

std::vector<uint16_t> MazeMemory::astarGetShortest(const uint16_t &current_path, const uint16_t &next_path)
{
    auto heuristic = [this](const uint16_t &a, const uint16_t &b)
    {
        uint8_t x1 = 0, y1 = 0;
        uint8_t x2 = 0, y2 = 0;
        Mouse_Utils::decode_coordinates(a, x1, y1);
        Mouse_Utils::decode_coordinates(b, x2, y2);
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    };

    std::priority_queue<std::pair<uint16_t, uint16_t>, std::vector<std::pair<uint16_t, uint16_t>>, std::greater<>> open_set;
    open_set.push({0, current_path});

    std::map<uint16_t, uint16_t> came_from;
    std::map<uint16_t, uint16_t> g_score;
    g_score[current_path] = 0;

    std::map<uint16_t, uint16_t> f_score;
    f_score[current_path] = heuristic(current_path, next_path);

    while (!open_set.empty())
    {
        auto top = open_set.top();
        auto current = top.second;
        open_set.pop();

        if (current == next_path)
        {
            std::vector<uint16_t> path;
            while (came_from.find(current) != came_from.end())
            {
                path.push_back(current);
                current = came_from[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        uint8_t x = 0, y = 0;
        Mouse_Utils::decode_coordinates(current, x, y);
        for (const auto &neighbor : getPossibleNodes(x, y))
        {
            int tentative_g_score = g_score[current] + 1; //calculateCostFunction(came_from, current);
            if (!g_score.count(neighbor) || tentative_g_score < g_score[neighbor])
            {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = tentative_g_score + heuristic(neighbor, next_path);
                open_set.push({f_score[neighbor], neighbor});
            }
        }
    }

    return {};
}

uint16_t MazeMemory::calculateCostFunction(std::map<uint16_t, uint16_t> came_from, uint16_t current_path)
{
    int cost = 10;
    bool is_turn_required = false;
    auto previous_first = came_from[current_path];
    auto previous_second = came_from[previous_first];
    if (Mouse_Utils::is_diagonal(current_path, previous_second))
    {
        // std::cout << "is diagonal true start" << std::endl;
        Mouse_Utils::printCoordinates(current_path);
        Mouse_Utils::printCoordinates(previous_second);
        // std::cout << "is diagonal end" << std::endl;
        cost = 1;
    }
    ////do something
    return cost;
}

void MazeMemory::updateObstrucleMemory(uint8_t x, uint8_t y, uint8_t value)
{
    memory[Mouse_Utils::encode_coordinates(x, y)] = value;
}

void MazeMemory::updateSavedObstrucleMemory()
{
    //0,0 left: 1, bottom: 1, right: 0, top: 1
    memory[0] = 13;
    //1,0 left: 0, bottom: 1, right: 0, top: 1
    memory[256] = 5;
    //2,0 left: 0, bottom: 0, right: 0, top: 1
    memory[512] = 1;
    //3,0 left: 0, bottom: 1, right: 0, top: 1
    memory[768] = 5;
    //4,0 left: 0, bottom: 1, right: 0, top: 1
    memory[1024] = 5;
    //5,0 left: 0, bottom: 1, right: 0, top: 1
    memory[1280] = 5;
    //0,1 left: 1, bottom: 0, right: 0, top: 1
    memory[1] = 9;
    //1,1 left: 0, bottom: 1, right: 0, top: 1
    memory[257] = 5;
    //2,1 left: 0, bottom: 1, right: 0, top: 0
    memory[513] = 4;
    //3,1 left: 0, bottom: 1, right: 0, top: 1
    memory[769] = 5;
    //4,1 left: 0, bottom: 1, right: 0, top: 1
    memory[1025] = 5;
    //5,1 left: 0, bottom: 1, right: 0, top: 1
    memory[1281] = 5;
    //0,2 left: 1, bottom: 0, right: 1, top: 0
    memory[2] = 10;
    //1,2 left: 1, bottom: 0, right: 0, top: 1
    memory[258] = 9;
    //2,2 left: 0, bottom: 1, right: 0, top: 1
    memory[514] = 5;
    //3,2 left: 0, bottom: 0, right: 1, top: 1
    memory[770] = 3;
    //4,2 left: 1, bottom: 0, right: 0, top: 1
    memory[1026] = 9;
    //5,2 left: 0, bottom: 1, right: 0, top: 1
    memory[1282] = 5;
    //0,3 left: 1, bottom: 0, right: 1, top: 0
    memory[3] = 10;
    //1,3 left: 1, bottom: 0, right: 1, top: 0
    memory[259] = 10;
    //2,3 left: 1, bottom: 0, right: 1, top: 1
    memory[515] = 11;
    //3,3 left: 1, bottom: 0, right: 1, top: 0
    memory[771] = 10;
    //4,3 left: 1, bottom: 0, right: 1, top: 0
    memory[1027] = 10;
    //5,3 left: 1, bottom: 0, right: 0, top: 1
    memory[1283] = 9;
    //0,4 left: 1, bottom: 0, right: 1, top: 0
    memory[4] = 10;
    //1,4 left: 1, bottom: 0, right: 1, top: 0
    memory[260] = 10;
    //2,4 left: 1, bottom: 0, right: 1, top: 0
    memory[516] = 10;
    //3,4 left: 1, bottom: 0, right: 1, top: 0
    memory[772] = 10;
    //4,4 left: 1, bottom: 0, right: 1, top: 0
    memory[1028] = 10;
    //5,4 left: 1, bottom: 0, right: 1, top: 0
    memory[1284] = 10;
    //0,5 left: 1, bottom: 0, right: 1, top: 0
    memory[5] = 10;
    //1,5 left: 1, bottom: 0, right: 1, top: 0
    memory[261] = 10;
    //2,5 left: 1, bottom: 0, right: 1, top: 0
    memory[517] = 10;
    //3,5 left: 1, bottom: 1, right: 0, top: 0
    memory[773] = 12;
    //4,5 left: 0, bottom: 1, right: 1, top: 0
    memory[1029] = 6;
    //5,5 left: 1, bottom: 1, right: 0, top: 0
    memory[1285] = 12;
    //6,0 left: 0, bottom: 1, right: 0, top: 1
    memory[1536] = 5;
    //7,0 left: 0, bottom: 1, right: 0, top: 1
    memory[1792] = 5;
    //8,0 left: 0, bottom: 1, right: 0, top: 1
    memory[2048] = 5;
    //9,0 left: 0, bottom: 1, right: 0, top: 1
    memory[2304] = 5;
    //10,0 left: 0, bottom: 1, right: 0, top: 1
    memory[2560] = 5;
    //11,0 left: 0, bottom: 1, right: 0, top: 1
    memory[2816] = 5;
    //12,0 left: 0, bottom: 1, right: 0, top: 1
    memory[3072] = 5;
    //13,0 left: 0, bottom: 1, right: 0, top: 1
    memory[3328] = 5;
    //14,0 left: 0, bottom: 1, right: 0, top: 1
    memory[3584] = 5;
    //15,0 left: 0, bottom: 0, right: 1, top: 1
    memory[3840] = 3;
    //6,1 left: 0, bottom: 1, right: 0, top: 1
    memory[1537] = 5;
    //7,1 left: 0, bottom: 1, right: 0, top: 1
    memory[1793] = 5;
    //8,1 left: 0, bottom: 1, right: 0, top: 1
    memory[2049] = 5;
    //9,1 left: 0, bottom: 1, right: 0, top: 1
    memory[2305] = 5;
    //10,1 left: 0, bottom: 1, right: 0, top: 1
    memory[2561] = 5;
    //11,1 left: 0, bottom: 1, right: 0, top: 1
    memory[2817] = 5;
    //12,1 left: 0, bottom: 1, right: 0, top: 1
    memory[3073] = 5;
    //13,1 left: 0, bottom: 1, right: 0, top: 1
    memory[3329] = 5;
    //14,1 left: 0, bottom: 0, right: 1, top: 1
    memory[3585] = 3;
    //15,1 left: 1, bottom: 0, right: 1, top: 0
    memory[3841] = 10;
    //6,2 left: 0, bottom: 1, right: 0, top: 1
    memory[1538] = 5;
    //7,2 left: 0, bottom: 1, right: 0, top: 1
    memory[1794] = 5;
    //8,2 left: 0, bottom: 0, right: 1, top: 1
    memory[2050] = 3;
    //9,2 left: 1, bottom: 0, right: 0, top: 1
    memory[2306] = 9;
    //10,2 left: 0, bottom: 1, right: 0, top: 1
    memory[2562] = 5;
    //11,2 left: 0, bottom: 1, right: 0, top: 1
    memory[2818] = 5;
    //12,2 left: 0, bottom: 1, right: 0, top: 1
    memory[3074] = 5;
    //13,2 left: 0, bottom: 0, right: 1, top: 1
    memory[3330] = 3;
    //14,2 left: 1, bottom: 0, right: 1, top: 0
    memory[3586] = 10;
    //15,2 left: 1, bottom: 0, right: 1, top: 0
    memory[3842] = 10;
    //6,3 left: 0, bottom: 0, right: 1, top: 1
    memory[1539] = 3;
    //7,3 left: 1, bottom: 0, right: 0, top: 1
    memory[1795] = 9;
    //8,3 left: 0, bottom: 1, right: 1, top: 0
    memory[2051] = 6;
    //9,3 left: 1, bottom: 0, right: 1, top: 0
    memory[2307] = 10;
    //10,3 left: 1, bottom: 0, right: 0, top: 1
    memory[2563] = 9;
    //11,3 left: 0, bottom: 0, right: 0, top: 1
    memory[2819] = 1;
    //12,3 left: 0, bottom: 1, right: 1, top: 1
    memory[3075] = 7;
    //13,3 left: 1, bottom: 0, right: 1, top: 0
    memory[3331] = 10;
    //14,3 left: 1, bottom: 0, right: 1, top: 0
    memory[3587] = 10;
    //15,3 left: 1, bottom: 0, right: 1, top: 0
    memory[3843] = 10;
    //6,4 left: 1, bottom: 1, right: 0, top: 0
    memory[1540] = 12;
    //7,4 left: 0, bottom: 1, right: 1, top: 0
    memory[1796] = 6;
    //8,4 left: 1, bottom: 0, right: 1, top: 1
    memory[2052] = 11;
    //9,4 left: 1, bottom: 1, right: 0, top: 0
    memory[2308] = 12;
    //10,4 left: 0, bottom: 1, right: 1, top: 0
    memory[2564] = 6;
    //11,4 left: 1, bottom: 1, right: 0, top: 0
    memory[2820] = 12;
    //12,4 left: 0, bottom: 0, right: 1, top: 1
    memory[3076] = 3;
    //13,4 left: 1, bottom: 0, right: 1, top: 0
    memory[3332] = 10;
    //14,4 left: 1, bottom: 0, right: 1, top: 0
    memory[3588] = 10;
    //15,4 left: 1, bottom: 0, right: 1, top: 0
    memory[3844] = 10;
    //6,5 left: 0, bottom: 0, right: 0, top: 1
    memory[1541] = 1;
    //7,5 left: 0, bottom: 1, right: 0, top: 1
    memory[1797] = 5;
    //8,5 left: 0, bottom: 1, right: 0, top: 0
    memory[2053] = 4;
    //9,5 left: 0, bottom: 1, right: 0, top: 1
    memory[2309] = 5;
    //10,5 left: 0, bottom: 1, right: 0, top: 1
    memory[2565] = 5;
    //11,5 left: 0, bottom: 1, right: 0, top: 1
    memory[2821] = 5;
    //12,5 left: 0, bottom: 1, right: 1, top: 0
    memory[3077] = 6;
    //13,5 left: 1, bottom: 0, right: 1, top: 0
    memory[3333] = 10;
    //14,5 left: 1, bottom: 0, right: 1, top: 0
    memory[3589] = 10;
    //15,5 left: 1, bottom: 0, right: 1, top: 0
    memory[3845] = 10;
    //0,6 left: 1, bottom: 0, right: 1, top: 0
    memory[6] = 10;
    //1,6 left: 1, bottom: 1, right: 0, top: 0
    memory[262] = 12;
    //2,6 left: 0, bottom: 1, right: 0, top: 0
    memory[518] = 4;
    //3,6 left: 0, bottom: 1, right: 0, top: 1
    memory[774] = 5;
    //4,6 left: 0, bottom: 1, right: 0, top: 1
    memory[1030] = 5;
    //5,6 left: 0, bottom: 0, right: 1, top: 1
    memory[1286] = 3;
    //6,6 left: 1, bottom: 1, right: 0, top: 0
    memory[1542] = 12;
    //7,6 left: 0, bottom: 1, right: 0, top: 1
    memory[1798] = 5;
    //8,6 left: 0, bottom: 1, right: 0, top: 1
    memory[2054] = 5;
    //9,6 left: 0, bottom: 1, right: 0, top: 1
    memory[2310] = 5;
    //10,6 left: 0, bottom: 1, right: 0, top: 1
    memory[2566] = 5;
    //11,6 left: 0, bottom: 0, right: 1, top: 1
    memory[2822] = 3;
    //12,6 left: 1, bottom: 0, right: 0, top: 1
    memory[3078] = 9;
    //13,6 left: 0, bottom: 0, right: 1, top: 0
    memory[3334] = 2;
    //14,6 left: 1, bottom: 0, right: 1, top: 0
    memory[3590] = 10;
    //15,6 left: 1, bottom: 0, right: 1, top: 0
    memory[3846] = 10;
    //0,7 left: 1, bottom: 0, right: 1, top: 0
    memory[7] = 10;
    //1,7 left: 1, bottom: 0, right: 0, top: 1
    memory[263] = 9;
    //2,7 left: 0, bottom: 1, right: 0, top: 1
    memory[519] = 5;
    //3,7 left: 0, bottom: 1, right: 0, top: 1
    memory[775] = 5;
    //4,7 left: 0, bottom: 1, right: 0, top: 1
    memory[1031] = 5;
    //5,7 left: 0, bottom: 1, right: 1, top: 0
    memory[1287] = 6;
    //6,7 left: 1, bottom: 0, right: 1, top: 1
    memory[1543] = 11;
    //7,7 left: 1, bottom: 0, right: 0, top: 1
    memory[1799] = 9;
    //8,7 left: 0, bottom: 0, right: 1, top: 1
    memory[2055] = 3;
    //9,7 left: 1, bottom: 0, right: 1, top: 1
    memory[2311] = 11;
    //10,7 left: 1, bottom: 0, right: 0, top: 1
    memory[2567] = 9;
    //11,7 left: 0, bottom: 1, right: 1, top: 0
    memory[2823] = 6;
    //12,7 left: 1, bottom: 0, right: 1, top: 0
    memory[3079] = 10;
    //13,7 left: 1, bottom: 0, right: 1, top: 0
    memory[3335] = 10;
    //14,7 left: 1, bottom: 0, right: 1, top: 0
    memory[3591] = 10;
    //15,7 left: 1, bottom: 0, right: 1, top: 0
    memory[3847] = 10;
    //0,8 left: 1, bottom: 0, right: 1, top: 0
    memory[8] = 10;
    //1,8 left: 1, bottom: 0, right: 1, top: 0
    memory[264] = 10;
    //2,8 left: 1, bottom: 0, right: 1, top: 1
    memory[520] = 11;
    //3,8 left: 1, bottom: 0, right: 1, top: 1
    memory[776] = 11;
    //4,8 left: 1, bottom: 0, right: 0, top: 1
    memory[1032] = 9;
    //5,8 left: 0, bottom: 1, right: 0, top: 1
    memory[1288] = 5;
    //6,8 left: 0, bottom: 0, right: 1, top: 0
    memory[1544] = 2;
    //7,8 left: 1, bottom: 1, right: 0, top: 0
    memory[1800] = 12;
    //8,8 left: 0, bottom: 0, right: 1, top: 0
    memory[2056] = 2;
    //9,8 left: 1, bottom: 0, right: 0, top: 0
    memory[2312] = 8;
    //10,8 left: 0, bottom: 1, right: 1, top: 0
    memory[2568] = 6;
    //11,8 left: 1, bottom: 0, right: 0, top: 1
    memory[2824] = 9;
    //12,8 left: 0, bottom: 1, right: 1, top: 0
    memory[3080] = 6;
    //13,8 left: 1, bottom: 1, right: 1, top: 0
    memory[3336] = 14;
    //14,8 left: 1, bottom: 0, right: 1, top: 0
    memory[3592] = 10;
    //15,8 left: 1, bottom: 0, right: 1, top: 0
    memory[3848] = 10;
    //0,9 left: 1, bottom: 0, right: 1, top: 0
    memory[9] = 10;
    //1,9 left: 1, bottom: 0, right: 1, top: 0
    memory[265] = 10;
    //2,9 left: 1, bottom: 0, right: 1, top: 0
    memory[521] = 10;
    //3,9 left: 1, bottom: 0, right: 0, top: 0
    memory[777] = 8;
    //4,9 left: 0, bottom: 1, right: 1, top: 0
    memory[1033] = 6;
    //5,9 left: 1, bottom: 0, right: 0, top: 1
    memory[1289] = 9;
    //6,9 left: 0, bottom: 1, right: 1, top: 0
    memory[1545] = 6;
    //7,9 left: 1, bottom: 0, right: 0, top: 1
    memory[1801] = 9;
    //8,9 left: 0, bottom: 1, right: 1, top: 0
    memory[2057] = 6;
    //9,9 left: 1, bottom: 0, right: 1, top: 0
    memory[2313] = 10;
    //10,9 left: 1, bottom: 0, right: 0, top: 1
    memory[2569] = 9;
    //11,9 left: 0, bottom: 1, right: 0, top: 0
    memory[2825] = 4;
    //12,9 left: 0, bottom: 1, right: 1, top: 1
    memory[3081] = 7;
    //13,9 left: 1, bottom: 0, right: 1, top: 1
    memory[3337] = 11;
    //14,9 left: 1, bottom: 0, right: 1, top: 0
    memory[3593] = 10;
    //15,9 left: 1, bottom: 0, right: 1, top: 0
    memory[3849] = 10;
    //0,10 left: 1, bottom: 0, right: 1, top: 0
    memory[10] = 10;
    //1,10 left: 1, bottom: 0, right: 1, top: 0
    memory[266] = 10;
    //2,10 left: 1, bottom: 0, right: 0, top: 0
    memory[522] = 8;
    //3,10 left: 0, bottom: 1, right: 1, top: 0
    memory[778] = 6;
    //4,10 left: 1, bottom: 0, right: 0, top: 1
    memory[1034] = 9;
    //5,10 left: 0, bottom: 1, right: 1, top: 0
    memory[1290] = 6;
    //6,10 left: 1, bottom: 0, right: 0, top: 1
    memory[1546] = 9;
    //7,10 left: 0, bottom: 0, right: 1, top: 0
    memory[1802] = 2;
    //8,10 left: 1, bottom: 0, right: 0, top: 1
    memory[2058] = 9;
    //9,10 left: 0, bottom: 1, right: 1, top: 0
    memory[2314] = 6;
    //10,10 left: 1, bottom: 1, right: 0, top: 0
    memory[2570] = 12;
    //11,10 left: 0, bottom: 1, right: 0, top: 1
    memory[2826] = 5;
    //12,10 left: 0, bottom: 0, right: 1, top: 1
    memory[3082] = 3;
    //13,10 left: 1, bottom: 0, right: 0, top: 0
    memory[3338] = 8;
    //14,10 left: 0, bottom: 0, right: 1, top: 0
    memory[3594] = 2;
    //15,10 left: 1, bottom: 0, right: 1, top: 0
    memory[3850] = 10;
    //0,11 left: 1, bottom: 0, right: 1, top: 0
    memory[11] = 10;
    //1,11 left: 1, bottom: 1, right: 0, top: 0
    memory[267] = 12;
    //2,11 left: 0, bottom: 1, right: 1, top: 0
    memory[523] = 6;
    //3,11 left: 1, bottom: 0, right: 0, top: 1
    memory[779] = 9;
    //4,11 left: 0, bottom: 1, right: 1, top: 0
    memory[1035] = 6;
    //5,11 left: 1, bottom: 0, right: 0, top: 1
    memory[1291] = 9;
    //6,11 left: 0, bottom: 1, right: 1, top: 0
    memory[1547] = 6;
    //7,11 left: 1, bottom: 1, right: 1, top: 0
    memory[1803] = 14;
    //8,11 left: 1, bottom: 0, right: 1, top: 0
    memory[2059] = 10;
    //9,11 left: 1, bottom: 0, right: 0, top: 1
    memory[2315] = 9;
    //10,11 left: 0, bottom: 0, right: 1, top: 1
    memory[2571] = 3;
    //11,11 left: 1, bottom: 0, right: 1, top: 1
    memory[2827] = 11;
    //12,11 left: 1, bottom: 1, right: 0, top: 0
    memory[3083] = 12;
    //13,11 left: 0, bottom: 0, right: 1, top: 0
    memory[3339] = 2;
    //14,11 left: 1, bottom: 0, right: 1, top: 0
    memory[3595] = 10;
    //15,11 left: 1, bottom: 0, right: 1, top: 0
    memory[3851] = 10;
    //0,12 left: 1, bottom: 0, right: 1, top: 0
    memory[12] = 10;
    //1,12 left: 1, bottom: 0, right: 1, top: 1
    memory[268] = 11;
    //2,12 left: 1, bottom: 0, right: 0, top: 1
    memory[524] = 9;
    //3,12 left: 0, bottom: 1, right: 1, top: 0
    memory[780] = 6;
    //4,12 left: 1, bottom: 0, right: 0, top: 1
    memory[1036] = 9;
    //5,12 left: 0, bottom: 0, right: 0, top: 0
    memory[1292] = 0;
    //6,12 left: 0, bottom: 1, right: 0, top: 1
    memory[1548] = 5;
    //7,12 left: 0, bottom: 0, right: 1, top: 1
    memory[1804] = 3;
    //8,12 left: 1, bottom: 0, right: 1, top: 0
    memory[2060] = 10;
    //9,12 left: 1, bottom: 0, right: 1, top: 0
    memory[2316] = 10;
    //10,12 left: 1, bottom: 0, right: 1, top: 0
    memory[2572] = 10;
    //11,12 left: 1, bottom: 0, right: 0, top: 0
    memory[2828] = 8;
    //12,12 left: 0, bottom: 0, right: 1, top: 1
    memory[3084] = 3;
    //13,12 left: 1, bottom: 0, right: 1, top: 0
    memory[3340] = 10;
    //14,12 left: 1, bottom: 0, right: 1, top: 0
    memory[3596] = 10;
    //15,12 left: 1, bottom: 0, right: 1, top: 0
    memory[3852] = 10;
    //0,13 left: 1, bottom: 0, right: 1, top: 0
    memory[13] = 10;
    //1,13 left: 1, bottom: 0, right: 0, top: 0
    memory[269] = 8;
    //2,13 left: 0, bottom: 1, right: 1, top: 0
    memory[525] = 6;
    //3,13 left: 1, bottom: 0, right: 0, top: 1
    memory[781] = 9;
    //4,13 left: 0, bottom: 1, right: 1, top: 0
    memory[1037] = 6;
    //5,13 left: 1, bottom: 0, right: 1, top: 0
    memory[1293] = 10;
    //6,13 left: 1, bottom: 1, right: 0, top: 1
    memory[1549] = 13;
    //7,13 left: 0, bottom: 0, right: 1, top: 0
    memory[1805] = 2;
    //8,13 left: 1, bottom: 1, right: 0, top: 0
    memory[2061] = 12;
    //9,13 left: 0, bottom: 1, right: 1, top: 0
    memory[2317] = 6;
    //10,13 left: 1, bottom: 0, right: 1, top: 0
    memory[2573] = 10;
    //11,13 left: 1, bottom: 0, right: 1, top: 0
    memory[2829] = 10;
    //12,13 left: 1, bottom: 0, right: 1, top: 0
    memory[3085] = 10;
    //13,13 left: 1, bottom: 0, right: 1, top: 0
    memory[3341] = 10;
    //14,13 left: 1, bottom: 0, right: 1, top: 0
    memory[3597] = 10;
    //15,13 left: 1, bottom: 0, right: 1, top: 0
    memory[3853] = 10;
    //0,14 left: 1, bottom: 0, right: 1, top: 0
    memory[14] = 10;
    //1,14 left: 1, bottom: 1, right: 0, top: 0
    memory[270] = 12;
    //2,14 left: 0, bottom: 1, right: 0, top: 1
    memory[526] = 5;
    //3,14 left: 0, bottom: 1, right: 0, top: 0
    memory[782] = 4;
    //4,14 left: 0, bottom: 1, right: 0, top: 1
    memory[1038] = 5;
    //5,14 left: 0, bottom: 1, right: 0, top: 0
    memory[1294] = 4;
    //6,14 left: 0, bottom: 1, right: 0, top: 1
    memory[1550] = 5;
    //7,14 left: 0, bottom: 1, right: 0, top: 0
    memory[1806] = 4;
    //8,14 left: 0, bottom: 1, right: 0, top: 1
    memory[2062] = 5;
    //9,14 left: 0, bottom: 1, right: 1, top: 1
    memory[2318] = 7;
    //10,14 left: 1, bottom: 1, right: 0, top: 0
    memory[2574] = 12;
    //11,14 left: 0, bottom: 1, right: 1, top: 0
    memory[2830] = 6;
    //12,14 left: 1, bottom: 1, right: 0, top: 0
    memory[3086] = 12;
    //13,14 left: 0, bottom: 1, right: 1, top: 0
    memory[3342] = 6;
    //14,14 left: 1, bottom: 0, right: 1, top: 0
    memory[3598] = 10;
    //15,14 left: 1, bottom: 0, right: 1, top: 0
    memory[3854] = 10;
    //0,15 left: 1, bottom: 1, right: 0, top: 0
    memory[15] = 12;
    //1,15 left: 0, bottom: 1, right: 0, top: 1
    memory[271] = 5;
    //2,15 left: 0, bottom: 1, right: 0, top: 1
    memory[527] = 5;
    //3,15 left: 0, bottom: 1, right: 0, top: 1
    memory[783] = 5;
    //4,15 left: 0, bottom: 1, right: 0, top: 1
    memory[1039] = 5;
    //5,15 left: 0, bottom: 1, right: 0, top: 1
    memory[1295] = 5;
    //6,15 left: 0, bottom: 1, right: 0, top: 1
    memory[1551] = 5;
    //7,15 left: 0, bottom: 1, right: 0, top: 1
    memory[1807] = 5;
    //8,15 left: 0, bottom: 1, right: 0, top: 1
    memory[2063] = 5;
    //9,15 left: 0, bottom: 1, right: 0, top: 1
    memory[2319] = 5;
    //10,15 left: 0, bottom: 1, right: 0, top: 1
    memory[2575] = 5;
    //11,15 left: 0, bottom: 1, right: 0, top: 1
    memory[2831] = 5;
    //12,15 left: 0, bottom: 1, right: 0, top: 1
    memory[3087] = 5;
    //13,15 left: 0, bottom: 1, right: 0, top: 1
    memory[3343] = 5;
    //14,15 left: 0, bottom: 1, right: 0, top: 0
    memory[3599] = 4;
    //15,15 left: 0, bottom: 1, right: 1, top: 0
    memory[3855] = 6;
}
