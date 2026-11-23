#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>

class MazeMemory {
private:
    int row;
    int column;
    std::map<std::tuple<int, int>, int> memory;
    void create_initial_memory();
    std::set<std::tuple<int, int>> visited;
    std::map<std::tuple<int, int>, std::map<std::string, bool>> obstructle_memory;
    std::vector<std::tuple<int, int>> strictly_prohibited_memory;
    void __update_strictly_prohibited();
    std::vector<std::pair<std::tuple<int, int>, int>> solve_sorting(std::vector<std::pair<std::tuple<int, int>, int>> pathCurrent, int memory);
    static MazeMemory* m_mazememory;
public:
    MazeMemory(int row, int column);
    std::vector<std::tuple<int, int>> get_next_shortest(int x, int y, std::map<std::string, bool> current_grid);
    bool is_destination(int x, int y);
    void add_visited(std::tuple<int, int> coordinates);
    bool check_if_visited(std::tuple<int, int> coordinates);
    std::vector<std::tuple<int, int>> get_next_shortest_from_learning(std::tuple<int, int> coordinates);
    void update_shortest_coordinates_calculation(std::vector<std::tuple<int, int>> path, int start_value = -1);
    std::vector<std::vector<int>> visualize_memory();
    static MazeMemory* getInstance(int row, int column) {
        if (m_mazememory == nullptr) {
            m_mazememory = new MazeMemory(row, column);
        }
        return m_mazememory;
    }
};
