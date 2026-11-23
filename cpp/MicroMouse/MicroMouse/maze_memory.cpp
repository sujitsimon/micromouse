#include "maze_memory.h"

MazeMemory* MazeMemory::m_mazememory = nullptr;

MazeMemory::MazeMemory(int row, int column) {
    this->row = row;
    this->column = column;
    this->create_initial_memory();
    this->__update_strictly_prohibited();
}

void MazeMemory::create_initial_memory() {
    assert(this->row == this->column && "Only Square Matrix Supported");
    int max_value = this->row - 2;
    for (int i = 0; i < this->row / 2; i++) {
        for (int j = 0; j < this->column / 2; j++) {
            this->memory[std::make_tuple(i, j)] = max_value - i - j;
            this->memory[std::make_tuple(this->row - i - 1, j)] = max_value - i - j;
            this->memory[std::make_tuple(i, this->column - j - 1)] = max_value - i - j;
            this->memory[std::make_tuple(this->row - i - 1, this->column - j - 1)] = max_value - i - j;
        }
    }
}

void MazeMemory::__update_strictly_prohibited() {
    int start_row = this->row / 2 - 1;
    int end_row = this->row / 2 + 1;
    int start_column = this->column / 2 - 1;
    int end_column = this->column / 2 + 1;
    for (int i = start_row; i < end_row; i++) {
        for (int j = start_column; j < end_column; j++) {
            this->strictly_prohibited_memory.push_back(std::make_tuple(i, j));
        }
    }
}

std::vector<std::pair<std::tuple<int, int>, int>> MazeMemory::solve_sorting(std::vector<std::pair<std::tuple<int, int>, int>> pathCurrent, int memory) {

}

std::vector<std::tuple<int, int>> MazeMemory::get_next_shortest(int x, int y, std::map<std::string, bool> current_grid) {
    this->obstructle_memory[std::make_tuple(x, y)] = current_grid;
    std::map<std::tuple<int, int>, int> minimum_path;
    /*std::cout << "Entered: (" << x << ", " << y << ")" << std::endl;
    std::cout << "left: "<< current_grid["left"]  << std::endl;
    std::cout << "right: " << current_grid["right"] << std::endl;
    std::cout << "top: " << current_grid["top"] << std::endl;
    std::cout << "bottom: " << current_grid["bottom"] << std::endl;*/
    //right
    if (x + 1 < this->row && current_grid["right"] == 0 && this->visited.find(std::make_tuple(x + 1, y)) == this->visited.end()) {
        minimum_path[std::make_tuple(x + 1, y)] = this->memory[std::make_tuple(x + 1, y)];
    }
    //left
    if (x - 1 >= 0 && current_grid["left"] == 0 && this->visited.find(std::make_tuple(x - 1, y)) == this->visited.end()) {
        minimum_path[std::make_tuple(x - 1, y)] = this->memory[std::make_tuple(x - 1, y)];
    }
    //top
    if (y - 1 >= 0 && current_grid["top"] == 0 && this->visited.find(std::make_tuple(x, y - 1)) == this->visited.end()) {
        minimum_path[std::make_tuple(x, y - 1)] = this->memory[std::make_tuple(x, y - 1)];
    }
    //bottom
    if (y + 1 < this->column && current_grid["bottom"] == 0 && this->visited.find(std::make_tuple(x, y + 1)) == this->visited.end()) {
        minimum_path[std::make_tuple(x, y + 1)] = this->memory[std::make_tuple(x, y + 1)];
    }
    auto compare = [](const auto& a, const auto& b) {
        return a.second > b.second;
    };

    std::vector<std::pair<std::tuple<int, int>, int>> sorted_minimum_path(minimum_path.begin(), minimum_path.end());
    std::sort(sorted_minimum_path.begin(), sorted_minimum_path.end(), compare);
    std::vector<std::tuple<int, int>> result;
    for (const auto& pair : sorted_minimum_path) {
        //std::cout << "(" << std::get<0>(pair.first) <<", " << std::get<1>(pair.first) << ")" << std::endl;
        result.push_back(std::make_tuple(std::get<0>(pair.first), std::get<1>(pair.first)));
    }
    return result;
}

bool MazeMemory::is_destination(int x, int y) {
    return this->memory[std::make_tuple(x, y)] == 0;
}

void MazeMemory::add_visited(std::tuple<int, int> coordinates) {
    this->visited.insert(coordinates);
}

bool MazeMemory::check_if_visited(std::tuple<int, int> coordinates) {
    return this->visited.find(coordinates) != this->visited.end();
}

std::vector<std::tuple<int, int>> MazeMemory::get_next_shortest_from_learning(std::tuple<int, int> coordinates) {
    std::vector<std::tuple<int, int>> path;
    while (true) {
        int value = this->memory[coordinates];
        path.push_back(coordinates);
        if (value == 0) {
            break;
        }
        if (this->memory.find(std::make_tuple(std::get<0>(coordinates) - 1, std::get<1>(coordinates))) != this->memory.end() && (value - this->memory[std::make_tuple(std::get<0>(coordinates) - 1, std::get<1>(coordinates))]) == 1 && this->check_if_visited(std::make_tuple(std::get<0>(coordinates) - 1, std::get<1>(coordinates)))) {
            coordinates = std::make_tuple(std::get<0>(coordinates) - 1, std::get<1>(coordinates));
            continue;
        }
        if (this->memory.find(std::make_tuple(std::get<0>(coordinates) + 1, std::get<1>(coordinates))) != this->memory.end() && (value - this->memory[std::make_tuple(std::get<0>(coordinates) + 1, std::get<1>(coordinates))]) == 1 && this->check_if_visited(std::make_tuple(std::get<0>(coordinates) + 1, std::get<1>(coordinates)))) {
            coordinates = std::make_tuple(std::get<0>(coordinates) + 1, std::get<1>(coordinates));
            continue;
        }
        if (this->memory.find(std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) - 1)) != this->memory.end() && (value - this->memory[std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) - 1)]) == 1 && this->check_if_visited(std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) - 1))) {
            coordinates = std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) - 1);
            continue;
        }
        if (this->memory.find(std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) + 1)) != this->memory.end() && (value - this->memory[std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) + 1)]) == 1 && this->check_if_visited(std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) + 1))) {
            coordinates = std::make_tuple(std::get<0>(coordinates), std::get<1>(coordinates) + 1);
            continue;
        }
        std::cout << path.size() << std::endl;
    }
    return path;
}

void MazeMemory::update_shortest_coordinates_calculation(std::vector<std::tuple<int, int>> path, int start_value) {
    while (!path.empty()) {
        if (start_value == -1) {
            start_value = this->memory[path.back()];
        }
        else {
            start_value++;
            this->memory[path.back()] = start_value;
        }
        path.pop_back();
    }
}

std::vector<std::vector<int>> MazeMemory::visualize_memory() {
    std::vector<std::vector<int>> mem_(this->row, std::vector<int>(this->column, 0));
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->column; j++) {
            mem_[i][j] = this->memory[std::make_tuple(j, i)];
        }
    }
    return mem_;
}

//int main() {
//    MazeMemory maze_mem_obj(6, 6);
//    maze_mem_obj.obstructle_memory = { {make_tuple(0, 0), {{"top", 1}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(0, 1), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(0, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(0, 3), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(0, 4), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(0, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 1}}},
//     {make_tuple(1, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(1, 1), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 0}}},
//     {make_tuple(1, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(1, 3), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(1, 4), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(1, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(2, 0), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(2, 1), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
//     {make_tuple(2, 2), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(2, 3), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(2, 4), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(2, 5), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
//     {make_tuple(3, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(3, 1), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(3, 2), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(3, 3), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(3, 4), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
//     {make_tuple(3, 5), {{"top", 1}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(4, 0), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(4, 1), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
//     {make_tuple(4, 2), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(4, 3), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
//     {make_tuple(4, 4), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
//     {make_tuple(4, 5), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
//     {make_tuple(5, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(5, 1), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(5, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
//     {make_tuple(5, 3), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(5, 4), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
//     {make_tuple(5, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 1}}} };
//    maze_mem_obj.update_shortest_coordinates_calculation({ make_tuple(3, 3) });
//    vector<vector<int>> mem = maze_mem_obj.visualize_memory();
//    vector<tuple<int, int>> sp = maze_mem_obj.strictly_prohibited_memory;
//    return 0;
//}
