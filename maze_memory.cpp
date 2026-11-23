#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

class MazeMemory {
private:
    int row;
    int column;
    map<pair<int, int>, int> memory;
    void create_initial_memory();
    set<pair<int, int>> visited;
    map<pair<int, int>, map<string, int>> obstructle_memory;
    vector<pair<int, int>> strictly_prohibited_memory;
    void __update_strictly_prohibited();
public:
    MazeMemory(int row, int column);
    vector<pair<int, int>> get_next_shortest(int x, int y, map<string, int> current_grid);
    bool is_destination(int x, int y);
    void add_visited(pair<int, int> coordinates);
    bool check_if_visited(pair<int, int> coordinates);
    vector<pair<int, int>> get_next_shortest_from_learning(pair<int, int> coordinates);
    void update_shortest_coordinates_calculation(vector<pair<int, int>> path, int start_value = -1);
    vector<vector<int>> visualize_memory();
};

MazeMemory::MazeMemory(int row, int column) {
    this->row = row;
    this->column = column;
    this->create_initial_memory();
    this->__update_strictly_prohibited();
}

void MazeMemory::create_initial_memory() {
    assert(this->row == this->column && "Only Square Matrix Supported");
    int max_value = this->row - 2;
    for (int i = 0; i < this->row/2; i++) {
        for (int j = 0; j < this->column/2; j++) {
            this->memory[make_pair(i, j)] = max_value - i - j;
            this->memory[make_pair(this->row - i - 1, j)] = max_value - i - j;
            this->memory[make_pair(i, this->column - j - 1)] = max_value - i - j;
            this->memory[make_pair(this->row - i - 1, this->column - j - 1)] = max_value - i - j;
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
            this->strictly_prohibited_memory.push_back(make_pair(i, j));
        }
    }
}

vector<pair<int, int>> MazeMemory::get_next_shortest(int x, int y, map<string, int> current_grid) {
    this->obstructle_memory[make_pair(x, y)] = current_grid;
    map<pair<int, int>, int> minimum_path;
    //left
    if (x - 1 >= 0 && current_grid["left"] == 0 && this->visited.find(make_pair(x-1, y)) == this->visited.end()) {
        minimum_path[make_pair(x-1, y)] = this->memory[make_pair(x-1, y)];
    }
    //right
    if (x + 1 < this->row && current_grid["right"] == 0 && this->visited.find(make_pair(x+1, y)) == this->visited.end()) {
        minimum_path[make_pair(x+1, y)] = this->memory[make_pair(x+1, y)];
    }
    //top
    if (y - 1 >= 0 && current_grid["top"] == 0 && this->visited.find(make_pair(x, y-1)) == this->visited.end()) {
        minimum_path[make_pair(x, y-1)] = this->memory[make_pair(x, y-1)];
    }
    //bottom
    if (y + 1 < this->column && current_grid["bottom"] == 0 && this->visited.find(make_pair(x, y+1)) == this->visited.end()) {
        minimum_path[make_pair(x, y+1)] = this->memory[make_pair(x, y+1)];
    }
    vector<pair<int, int>> result;
    for (auto it = minimum_path.rbegin(); it != minimum_path.rend(); it++) {
        result.push_back(it->first);
    }
    return result;
}

bool MazeMemory::is_destination(int x, int y) {
    return this->memory[make_pair(x, y)] == 0;
}

void MazeMemory::add_visited(pair<int, int> coordinates) {
    this->visited.insert(coordinates);
}

bool MazeMemory::check_if_visited(pair<int, int> coordinates) {
    return this->visited.find(coordinates) != this->visited.end();
}

vector<pair<int, int>> MazeMemory::get_next_shortest_from_learning(pair<int, int> coordinates) {
    vector<pair<int, int>> path;
    while (true) {
        int value = this->memory[coordinates];
        path.push_back(coordinates);
        if (value == 0) {
            break;
        }
        if (this->memory.find(make_pair(coordinates.first-1, coordinates.second)) != this->memory.end() && (value - this->memory[make_pair(coordinates.first-1, coordinates.second)]) == 1 && this->check_if_visited(make_pair(coordinates.first-1, coordinates.second))) {
            coordinates = make_pair(coordinates.first-1, coordinates.second);
            continue;
        }
        if (this->memory.find(make_pair(coordinates.first+1, coordinates.second)) != this->memory.end() && (value - this->memory[make_pair(coordinates.first+1, coordinates.second)]) == 1 && this->check_if_visited(make_pair(coordinates.first+1, coordinates.second))) {
            coordinates = make_pair(coordinates.first+1, coordinates.second);
            continue;
        }
        if (this->memory.find(make_pair(coordinates.first, coordinates.second-1)) != this->memory.end() && (value - this->memory[make_pair(coordinates.first, coordinates.second-1)]) == 1 && this->check_if_visited(make_pair(coordinates.first, coordinates.second-1))) {
            coordinates = make_pair(coordinates.first, coordinates.second-1);
            continue;
        }
        if (this->memory.find(make_pair(coordinates.first, coordinates.second+1)) != this->memory.end() && (value - this->memory[make_pair(coordinates.first, coordinates.second+1)]) == 1 && this->check_if_visited(make_pair(coordinates.first, coordinates.second+1))) {
            coordinates = make_pair(coordinates.first, coordinates.second+1);
            continue;
        }
        cout << path.size() << endl;
    }
    return path;
}

void MazeMemory::update_shortest_coordinates_calculation(vector<pair<int, int>> path, int start_value) {
    if (start_value == -1) {
        start_value = this->memory[path.back()];
    } else {
        start_value++;
        this->memory[path.back()] = start_value;
    }
    while (!path.empty()) {
        path.pop_back();
        if (start_value == 0) {
            break;
        }
        start_value++;
        this->memory[path.back()] = start_value;
    }
}

vector<vector<int>> MazeMemory::visualize_memory() {
    vector<vector<int>> mem_(this->row, vector<int>(this->column, 0));
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->column; j++) {
            mem_[i][j] = this->memory[make_pair(j, i)];
        }
    }
    return mem_;
}

int main() {
    MazeMemory maze_mem_obj(6, 6);
    maze_mem_obj.obstructle_memory = {{make_pair(0, 0), {{"top", 1}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(0, 1), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(0, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(0, 3), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(0, 4), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(0, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 1}}},
     {make_pair(1, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(1, 1), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 0}}},
     {make_pair(1, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(1, 3), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(1, 4), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(1, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(2, 0), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(2, 1), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
     {make_pair(2, 2), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(2, 3), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(2, 4), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(2, 5), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
     {make_pair(3, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(3, 1), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(3, 2), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(3, 3), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(3, 4), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
     {make_pair(3, 5), {{"top", 1}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(4, 0), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(4, 1), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
     {make_pair(4, 2), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(4, 3), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 0}}},
     {make_pair(4, 4), {{"top", 1}, {"bottom", 0}, {"left", 1}, {"right", 0}}},
     {make_pair(4, 5), {{"top", 0}, {"bottom", 1}, {"left", 0}, {"right", 1}}},
     {make_pair(5, 0), {{"top", 1}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(5, 1), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(5, 2), {{"top", 0}, {"bottom", 0}, {"left", 1}, {"right", 1}}},
     {make_pair(5, 3), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(5, 4), {{"top", 0}, {"bottom", 0}, {"left", 0}, {"right", 1}}},
     {make_pair(5, 5), {{"top", 0}, {"bottom", 1}, {"left", 1}, {"right", 1}}}};
    maze_mem_obj.update_shortest_coordinates_calculation({make_pair(3, 3)});
    vector<vector<int>> mem = maze_mem_obj.visualize_memory();
    vector<pair<int, int>> sp = maze_mem_obj.strictly_prohibited_memory;
    return 0;
}


