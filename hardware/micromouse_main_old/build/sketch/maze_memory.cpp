#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main_old\\maze_memory.cpp"
#include "maze_memory.h"

MazeMemory *MazeMemory::m_mazememory = nullptr;

MazeMemory::MazeMemory(uint8_t row, uint8_t column, uint8_t x_cooridantes_min, uint8_t y_cooridantes_min, uint8_t x_cooridantes_max, uint8_t y_cooridantes_max, bool stub, Logger *loggerInstance)
{
  this->row = row;
  this->column = column;
  this->x_cooridantes_min = x_cooridantes_min;
  this->y_cooridantes_min = y_cooridantes_min;
  this->x_cooridantes_max = x_cooridantes_max;
  this->y_cooridantes_max = y_cooridantes_max;
  this->create_initial_memory();
  this->m_logger = loggerInstance;
  // this->__update_strictly_prohibited();
  if (stub)
  {
    updateSavedObstrucleMemory();
  }
}

void MazeMemory::create_initial_memory()
{
  //    assert(this->row == this->column && "Only Square Matrix Supported");
  int8_t minimum_coordinates = std::min(x_cooridantes_min, y_cooridantes_min);
  int16_t value = (minimum_coordinates * 2) + abs(y_cooridantes_min - x_cooridantes_min);
  int8_t row_sign = -1;
  int8_t column_sign = -1;
  for (int i = 0; i < row; i++)
  {
    if (i > 0)
      value = memory[make_memory(i - 1, 0)] + row_sign;
    for (int j = 0; j < row; j++)
    {
      memory[make_memory(i, j)] = value;
      if (j == y_cooridantes_min)
        column_sign = 0;
      if (j == y_cooridantes_max)
        column_sign = 1;
      value += column_sign;
    }
    column_sign = -1;
    if (i == x_cooridantes_min)
      row_sign = 0;
    if (i == x_cooridantes_max)
      row_sign = 1;
  }
}

// void MazeMemory::__update_strictly_prohibited() {
//     uint8_t start_row = this->row / 2 - 1;
//     uint8_t end_row = this->row / 2 + 1;
//     uint8_t start_column = this->column / 2 - 1;
//     uint8_t end_column = this->column / 2 + 1;
//     for (uint8_t i = start_row; i < end_row; i++) {
//         for (uint8_t j = start_column; j < end_column; j++) {
//             this->strictly_prohibited_memory.push_back(std::make_tuple(i, j));
//         }
//     }
// }

uint8_t MazeMemory::make_memory(uint8_t x, uint8_t y)
{
  uint8_t memory_address = 0;
  memory_address |= x;
  memory_address = memory_address << 4;
  memory_address |= y;
  return memory_address;
}

uint8_t MazeMemory::parseX(uint8_t memory)
{
  return (memory & X_MASK) >> 4;
}

uint8_t MazeMemory::parseY(uint8_t memory)
{
  return (memory & Y_MASK);
}

std::vector<uint8_t> MazeMemory::get_next_shortest(uint8_t x, uint8_t y)
{
  uint8_t current_grid_int = this->obstructle_memory[make_memory(x, y)];
  std::map<std::string, bool> current_grid;
  current_grid["left"] = (current_grid_int & (1 << LEFT_SIDE)) > 0;
  current_grid["bottom"] = (current_grid_int & (1 << BOTTOM_SIDE)) > 0;
  current_grid["right"] = (current_grid_int & (1 << RIGHT_SIDE)) > 0;
  current_grid["top"] = (current_grid_int & (1 << TOP_SIDE)) > 0;
  std::map<uint8_t, uint8_t> minimum_path;
  // left
  if (x - 1 >= 0 && current_grid["left"] == 0 && this->visited.find(make_memory(x - 1, y)) == this->visited.end())
  {
    minimum_path[make_memory(x - 1, y)] = this->memory[make_memory(x - 1, y)];
  }
  // right
  if (x + 1 < this->row && current_grid["right"] == 0 && this->visited.find(make_memory(x + 1, y)) == this->visited.end())
  {
    minimum_path[make_memory(x + 1, y)] = this->memory[make_memory(x + 1, y)];
  }
  // top
  if (y - 1 >= 0 && current_grid["top"] == 0 && this->visited.find(make_memory(x, y - 1)) == this->visited.end())
  {
    minimum_path[make_memory(x, y - 1)] = this->memory[make_memory(x, y - 1)];
  }
  // bottom
  if (y + 1 < this->column && current_grid["bottom"] == 0 && this->visited.find(make_memory(x, y + 1)) == this->visited.end())
  {
    minimum_path[make_memory(x, y + 1)] = this->memory[make_memory(x, y + 1)];
  }
  auto compare = [](const std::pair<uint8_t, uint8_t> &a, const std::pair<uint8_t, uint8_t> &b)
  {
    return a.second > b.second;
  };

  std::vector<std::pair<uint8_t, uint8_t>> sorted_minimum_path(minimum_path.begin(), minimum_path.end());
  std::sort(sorted_minimum_path.begin(), sorted_minimum_path.end(), compare);
  std::vector<uint8_t> result;
  for (const auto &pair : sorted_minimum_path)
  {
    //std::cout << "(" << std::get<0>(pair.first) <<", " << std::get<1>(pair.first) << ")" << std::endl;
    result.push_back(pair.first);
  }
  return result;
}

bool MazeMemory::is_destination(uint8_t x, uint8_t y)
{
  return this->memory[make_memory(x, y)] == 0;
}

void MazeMemory::add_visited(uint8_t coordinates)
{
  this->visited.insert(coordinates);
}

bool MazeMemory::check_if_visited(uint8_t coordinates)
{
  return this->visited.find(coordinates) != this->visited.end();
}

std::vector<uint8_t> MazeMemory::get_next_shortest_from_learning(uint8_t coordinates)
{
  std::vector<uint8_t> path;
  //m_logger->print("get_next_shortest_from_learning called: ");
  //m_logger->println(coordinates);
  while (true)
  {
    uint8_t value = this->memory[coordinates];
    //m_logger->print("get_next_shortest_from_learning x: ");
    //m_logger->print(parseX(coordinates));
    //m_logger->print(" y: ");
    //m_logger->print(parseY(coordinates));
    //m_logger->print(" value: ");
    //m_logger->println(value);
    path.push_back(coordinates);
    if (value == 0)
    {
      break;
    }
    if (this->memory.find(make_memory(parseX(coordinates) - 1, parseY(coordinates))) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates) - 1, parseY(coordinates))]) == 1 && this->check_if_visited(make_memory(parseX(coordinates) - 1, parseY(coordinates))))
    {
      coordinates = make_memory(parseX(coordinates) - 1, parseY(coordinates));
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates) + 1, parseY(coordinates))) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates) + 1, parseY(coordinates))]) == 1 && this->check_if_visited(make_memory(parseX(coordinates) + 1, parseY(coordinates))))
    {
      coordinates = make_memory(parseX(coordinates) + 1, parseY(coordinates));
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates), parseY(coordinates) - 1)) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates), parseY(coordinates) - 1)]) == 1 && this->check_if_visited(make_memory(parseX(coordinates), parseY(coordinates) - 1)))
    {
      coordinates = make_memory(parseX(coordinates), parseY(coordinates) - 1);
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates), parseY(coordinates) + 1)) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates), parseY(coordinates) + 1)]) == 1 && this->check_if_visited(make_memory(parseX(coordinates), parseY(coordinates) + 1)))
    {
      coordinates = make_memory(parseX(coordinates), parseY(coordinates) + 1);
      continue;
    }
    // std::cout << path.size() << std::endl;
  }
  return path;
}

void MazeMemory::update_shortest_coordinates_calculation(std::vector<uint8_t> path, int start_value)
{
  uint8_t lastX;
  uint8_t lastY;
  while (!path.empty())
  {
    if (start_value == -1)
    {
      start_value = this->memory[path.back()];
    }
    else
    {
      start_value++;
      //m_logger->print("update_shortest_coordinates_calculation x: ");
      //m_logger->print(parseX(path.back()));
      //m_logger->print(" y: ");
      //m_logger->print(parseY(path.back()));
      //m_logger->print(" value: ");
      //m_logger->println(start_value);
      //m_logger->print("update_shortest_coordinates_calculation last x: ");
      //m_logger->print(lastX);
      //m_logger->print(" last y: ");
      //m_logger->println(lastY);
      if (!((lastX == parseX(path.back())) && (lastY == parseY(path.back()))))
      {
        this->memory[path.back()] = start_value;
      }
    }
    lastX = parseX(path.back());
    lastY = parseY(path.back());
    path.pop_back();
  }
}

// std::vector<std::vector<uint8_t>> MazeMemory::visualize_memory() {
//     std::vector<std::vector<uint8_t>> mem_(this->row, std::vector<uint8_t>(this->column, 0));
//     for (uint8_t i = 0; i < this->row; i++) {
//         for (uint8_t j = 0; j < this->column; j++) {
//             mem_[i][j] = this->memory[std::make_tuple(j, i)];
//         }
//     }
//     return mem_;
// }
void MazeMemory::updateObstrucleMemory(uint8_t x, uint8_t y, uint8_t current_grid)
{
  this->obstructle_memory[make_memory(x, y)] = current_grid;
}

void MazeMemory::printMemory()
{
  //m_logger->println("Current Maze Memory: ");
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < column; j++)
    {
      //m_logger->print(memory[make_memory(j, i)]);
      //m_logger->print("\t");
    }
    //m_logger->println();
  }
}

void MazeMemory::updateSavedObstrucleMemory()
{
  // 0,0 left: 1, bottom: 0, right: 0, top: 1
  obstructle_memory[0] = 9;
  // 0,1 left: 1, bottom: 0, right: 1, top: 0
  obstructle_memory[1] = 10;
  // 0,2 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[2] = 12;
  // 0,3 left: 1, bottom: 0, right: 0, top: 1
  obstructle_memory[3] = 9;
  // 0,4 left: 1, bottom: 0, right: 1, top: 0
  obstructle_memory[4] = 10;
  // 0,5 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[5] = 12;
  // 1,0 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[16] = 3;
  // 1,1 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[17] = 12;
  // 1,2 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[18] = 3;
  // 1,3 left: 0, bottom: 1, right: 1, top: 0
  obstructle_memory[19] = 6;
  // 1,4 left: 1, bottom: 0, right: 0, top: 1
  obstructle_memory[20] = 9;
  // 1,5 left: 0, bottom: 1, right: 1, top: 0
  obstructle_memory[21] = 6;
  // 2,0 left: 1, bottom: 0, right: 0, top: 0
  obstructle_memory[32] = 8;
  // 2,1 left: 0, bottom: 1, right: 1, top: 0
  obstructle_memory[33] = 6;
  // 2,2 left: 1, bottom: 0, right: 0, top: 1
  obstructle_memory[34] = 9;
  // 2,3 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[35] = 12;
  // 2,4 left: 0, bottom: 1, right: 0, top: 1
  obstructle_memory[36] = 5;
  // 2,5 left: 1, bottom: 1, right: 0, top: 1
  obstructle_memory[37] = 13;
  // 3,0 left: 0, bottom: 0, right: 0, top: 1
  obstructle_memory[48] = 1;
  // 3,1 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[49] = 12;
  // 3,2 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[50] = 3;
  // 3,3 left: 0, bottom: 0, right: 1, top: 0
  obstructle_memory[51] = 2;
  // 3,4 left: 0, bottom: 1, right: 1, top: 0
  obstructle_memory[52] = 6;
  // 3,5 left: 0, bottom: 1, right: 0, top: 1
  obstructle_memory[53] = 5;
  // 4,0 left: 0, bottom: 1, right: 0, top: 1
  obstructle_memory[64] = 5;
  // 4,1 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[65] = 3;
  // 4,2 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[66] = 12;
  // 4,3 left: 1, bottom: 0, right: 1, top: 1
  obstructle_memory[67] = 11;
  // 4,4 left: 1, bottom: 1, right: 0, top: 0
  obstructle_memory[68] = 12;
  // 4,5 left: 0, bottom: 1, right: 0, top: 1
  obstructle_memory[69] = 5;
  // 5,0 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[80] = 3;
  // 5,1 left: 1, bottom: 1, right: 1, top: 0
  obstructle_memory[81] = 14;
  // 5,2 left: 0, bottom: 0, right: 1, top: 1
  obstructle_memory[82] = 3;
  // 5,3 left: 1, bottom: 0, right: 1, top: 0
  obstructle_memory[83] = 10;
  // 5,4 left: 0, bottom: 0, right: 1, top: 0
  obstructle_memory[84] = 2;
  // 5,5 left: 0, bottom: 1, right: 1, top: 0
  obstructle_memory[85] = 6;
}
