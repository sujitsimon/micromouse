#include "maze_memory.h"

MazeMemory* MazeMemory::m_mazememory = nullptr;

MazeMemory::MazeMemory(uint8_t row, uint8_t column, uint8_t min_row_dest, uint8_t min_col_dest, uint8_t max_row_dest, uint8_t max_col_dest, bool stub) {
  this->row = row;
  this->column = column;
  this->min_row_dest = min_row_dest;
  this->min_col_dest = min_col_dest;
  this->max_row_dest = max_row_dest;
  this->max_col_dest = max_col_dest;
  this->create_initial_memory();
  //this->__update_strictly_prohibited();
  if (stub) {
    updateSavedObstrucleMemory();
  }
}

void MazeMemory::create_initial_memory() {
  //    assert(this->row == this->column && "Only Square Matrix Supported");
  uint16_t value = min_row_dest * 2;
  int8_t row_sign = -1;
  int8_t column_sign = -1;
  for (int i = 0; i < row; i++) {
    if (i > 0)
      value = this->memory[make_memory(i - 1, 0)] + row_sign;
    for (int j = 0; j < row; j++) {
      this->memory[make_memory(i, j)] = value;
      if (j == min_col_dest)
        column_sign = 0;
      if (j == max_col_dest)
        column_sign = 1;
      value += column_sign;
    }
    column_sign = -1;
    if (i == min_row_dest)
      row_sign = 0;
    if (i == max_row_dest)
      row_sign = 1;
  }
}

//void MazeMemory::__update_strictly_prohibited() {
//    uint8_t start_row = this->row / 2 - 1;
//    uint8_t end_row = this->row / 2 + 1;
//    uint8_t start_column = this->column / 2 - 1;
//    uint8_t end_column = this->column / 2 + 1;
//    for (uint8_t i = start_row; i < end_row; i++) {
//        for (uint8_t j = start_column; j < end_column; j++) {
//            this->strictly_prohibited_memory.push_back(std::make_tuple(i, j));
//        }
//    }
//}

uint8_t MazeMemory::make_memory(uint8_t x, uint8_t y) {
  uint8_t memory_address = 0;
  memory_address |= x;
  memory_address = memory_address << 4;
  memory_address |= y;
  return memory_address;
}

uint8_t MazeMemory::parseX(uint8_t memory) {
  return (memory & X_MASK) >> 4;
}

uint8_t MazeMemory::parseY(uint8_t memory) {
  return (memory & Y_MASK);
}

std::vector<uint8_t> MazeMemory::get_next_shortest(uint8_t x, uint8_t y) {
  uint8_t current_grid_int = this->obstructle_memory[make_memory(x, y)];
  std::map<std::string, bool> current_grid;
  current_grid["left"] = (current_grid_int & (1 << LEFT_SIDE)) > 0;
  current_grid["bottom"] = (current_grid_int & (1 << BOTTOM_SIDE)) > 0;
  current_grid["right"] = (current_grid_int & (1 << RIGHT_SIDE)) > 0;
  current_grid["top"] = (current_grid_int & (1 << TOP_SIDE)) > 0;
  std::map<uint8_t, uint8_t> minimum_path;
  //left
  if (x - 1 >= 0 && current_grid["left"] == 0 && this->visited.find(make_memory(x - 1, y)) == this->visited.end()) {
    minimum_path[make_memory(x - 1, y)] = this->memory[make_memory(x - 1, y)];
  }
  //right
  if (x + 1 < this->row && current_grid["right"] == 0 && this->visited.find(make_memory(x + 1, y)) == this->visited.end()) {
    minimum_path[make_memory(x + 1, y)] = this->memory[make_memory(x + 1, y)];
  }
  //top
  if (y - 1 >= 0 && current_grid["top"] == 0 && this->visited.find(make_memory(x, y - 1)) == this->visited.end()) {
    minimum_path[make_memory(x, y - 1)] = this->memory[make_memory(x, y - 1)];
  }
  //bottom
  if (y + 1 < this->column && current_grid["bottom"] == 0 && this->visited.find(make_memory(x, y + 1)) == this->visited.end()) {
    minimum_path[make_memory(x, y + 1)] = this->memory[make_memory(x, y + 1)];
  }
  auto compare = [](const std::pair<uint8_t, uint8_t>& a, const std::pair<uint8_t, uint8_t>& b) {
    return a.second > b.second;
  };

  std::vector<std::pair<uint8_t, uint8_t>> sorted_minimum_path(minimum_path.begin(), minimum_path.end());
  std::sort(sorted_minimum_path.begin(), sorted_minimum_path.end(), compare);
  std::vector<uint8_t> result;
  for (const auto& pair : sorted_minimum_path) {
    ////std::cout << "(" << std::get<0>(pair.first) <<", " << std::get<1>(pair.first) << ")" << std::endl;
    result.push_back(pair.first);
  }
  return result;
}

bool MazeMemory::is_destination(uint8_t x, uint8_t y) {
//  if ((x==1) && (y==2)) {
//    return true;
//  }
//  return false;
  return this->memory[make_memory(x, y)] == 0;
}

void MazeMemory::add_visited(uint8_t coordinates) {
  this->visited.insert(coordinates);
}

bool MazeMemory::check_if_visited(uint8_t coordinates) {
  return this->visited.find(coordinates) != this->visited.end();
}

std::vector<uint8_t> MazeMemory::get_next_shortest_from_learning(uint8_t coordinates) {
  std::vector<uint8_t> path;
  Serial.print("get_next_shortest_from_learning called: ");
  Serial.println(coordinates);
  while (true) {
    uint8_t value = this->memory[coordinates];
    Serial.print("get_next_shortest_from_learning x: ");
    Serial.print(parseX(coordinates));
    Serial.print(" y: ");
    Serial.print(parseY(coordinates));
    Serial.print(" value: ");
    Serial.println(value);
    path.push_back(coordinates);
    if (value == 0) {
      break;
    }
    if (this->memory.find(make_memory(parseX(coordinates) - 1, parseY(coordinates))) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates) - 1, parseY(coordinates))]) == 1 && this->check_if_visited(make_memory(parseX(coordinates) - 1, parseY(coordinates)))) {
      coordinates = make_memory(parseX(coordinates) - 1, parseY(coordinates));
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates) + 1, parseY(coordinates))) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates) + 1, parseY(coordinates))]) == 1 && this->check_if_visited(make_memory(parseX(coordinates) + 1, parseY(coordinates)))) {
      coordinates = make_memory(parseX(coordinates) + 1, parseY(coordinates));
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates), parseY(coordinates) - 1)) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates), parseY(coordinates) - 1)]) == 1 && this->check_if_visited(make_memory(parseX(coordinates), parseY(coordinates) - 1))) {
      coordinates = make_memory(parseX(coordinates), parseY(coordinates) - 1);
      continue;
    }
    if (this->memory.find(make_memory(parseX(coordinates), parseY(coordinates) + 1)) != this->memory.end() && (value - this->memory[make_memory(parseX(coordinates), parseY(coordinates) + 1)]) == 1 && this->check_if_visited(make_memory(parseX(coordinates), parseY(coordinates) + 1))) {
      coordinates = make_memory(parseX(coordinates), parseY(coordinates) + 1);
      continue;
    }
    //std::cout << path.size() << std::endl;
  }
  return path;
}

void MazeMemory::update_shortest_coordinates_calculation(std::vector<uint8_t> path, int start_value) {
  uint8_t lastX;
  uint8_t lastY;
  while (!path.empty()) {
    if (start_value == -1) {
      start_value = this->memory[path.back()];
    }
    else {
      start_value++;
      Serial.print("update_shortest_coordinates_calculation x: ");
      Serial.print(parseX(path.back()));
      Serial.print(" y: ");
      Serial.print(parseY(path.back()));
      Serial.print(" value: ");
      Serial.println(start_value);
      Serial.print("update_shortest_coordinates_calculation last x: ");
      Serial.print(lastX);
      Serial.print(" last y: ");
      Serial.print(lastY);
      if (!((lastX == parseX(path.back())) && (lastY == parseY(path.back())))) {
        this->memory[path.back()] = start_value;
      }
    }
    lastX = parseX(path.back());
    lastY = parseY(path.back());
    path.pop_back();
  }
}

//std::vector<std::vector<uint8_t>> MazeMemory::visualize_memory() {
//    std::vector<std::vector<uint8_t>> mem_(this->row, std::vector<uint8_t>(this->column, 0));
//    for (uint8_t i = 0; i < this->row; i++) {
//        for (uint8_t j = 0; j < this->column; j++) {
//            mem_[i][j] = this->memory[std::make_tuple(j, i)];
//        }
//    }
//    return mem_;
//}
void MazeMemory::updateObstrucleMemory(uint8_t x, uint8_t y, uint8_t current_grid) {
  this->obstructle_memory[make_memory(x, y)] = current_grid;
}

void MazeMemory::updateSavedObstrucleMemory() {
  //0,0 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[0] = 11;
//0,1 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[1] = 10;
//0,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[2] = 10;
//0,3 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[3] = 10;
//0,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[4] = 10;
//0,5 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[5] = 10;
//0,6 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[6] = 12;
//0,7 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[7] = 9;
//0,8 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[8] = 14;
//0,9 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[9] = 9;
//0,10 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[10] = 12;
//0,11 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[11] = 11;
//0,12 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[12] = 10;
//0,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[13] = 10;
//0,14 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[14] = 12;
//0,15 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[15] = 13;
//1,0 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[16] = 9;
//1,1 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[17] = 8;
//1,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[18] = 10;
//1,3 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[19] = 8;
//1,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[20] = 10;
//1,5 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[21] = 10;
//1,6 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[22] = 6;
//1,7 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[23] = 3;
//1,8 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[24] = 10;
//1,9 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[25] = 4;
//1,10 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[26] = 3;
//1,11 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[27] = 10;
//1,12 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[28] = 10;
//1,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[29] = 10;
//1,14 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[30] = 6;
//1,15 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[31] = 5;
//2,0 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[32] = 5;
//2,1 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[33] = 5;
//2,2 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[34] = 11;
//2,3 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[35] = 2;
//2,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[36] = 10;
//2,5 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[37] = 10;
//2,6 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[38] = 10;
//2,7 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[39] = 8;
//2,8 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[40] = 14;
//2,9 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[41] = 3;
//2,10 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[42] = 8;
//2,11 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[43] = 10;
//2,12 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[44] = 12;
//2,13 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[45] = 9;
//2,14 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[46] = 10;
//2,15 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[47] = 4;
//3,0 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[48] = 5;
//3,1 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[49] = 3;
//3,2 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[50] = 12;
//3,3 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[51] = 9;
//3,4 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[52] = 12;
//3,5 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[53] = 11;
//3,6 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[54] = 12;
//3,7 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[55] = 5;
//3,8 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[56] = 9;
//3,9 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[57] = 10;
//3,10 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[58] = 2;
//3,11 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[59] = 14;
//3,12 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[60] = 5;
//3,13 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[61] = 3;
//3,14 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[62] = 12;
//3,15 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[63] = 5;
//4,0 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[64] = 1;
//4,1 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[65] = 12;
//4,2 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[66] = 7;
//4,3 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[67] = 5;
//4,4 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[68] = 3;
//4,5 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[69] = 12;
//4,6 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[70] = 3;
//4,7 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[71] = 6;
//4,8 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[72] = 1;
//4,9 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[73] = 14;
//4,10 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[74] = 9;
//4,11 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[75] = 10;
//4,12 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[76] = 2;
//4,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[77] = 10;
//4,14 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[78] = 6;
//4,15 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[79] = 5;
//5,0 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[80] = 7;
//5,1 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[81] = 3;
//5,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[82] = 10;
//5,3 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[83] = 2;
//5,4 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[84] = 12;
//5,5 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[85] = 5;
//5,6 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[86] = 9;
//5,7 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[87] = 12;
//5,8 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[88] = 5;
//5,9 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[89] = 9;
//5,10 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[90] = 6;
//5,11 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[91] = 13;
//5,12 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[92] = 9;
//5,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[93] = 10;
//5,14 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[94] = 10;
//5,15 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[95] = 6;
//6,0 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[96] = 9;
//6,1 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[97] = 12;
//6,2 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[98] = 9;
//6,3 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[99] = 10;
//6,4 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[100] = 6;
//6,5 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[101] = 3;
//6,6 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[102] = 6;
//6,7 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[103] = 3;
//6,8 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[104] = 6;
//6,9 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[105] = 1;
//6,10 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[106] = 12;
//6,11 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[107] = 3;
//6,12 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[108] = 6;
//6,13 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[109] = 9;
//6,14 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[110] = 12;
//6,15 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[111] = 13;
//7,0 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[112] = 6;
//7,1 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[113] = 1;
//7,2 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[114] = 6;
//7,3 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[115] = 9;
//7,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[116] = 10;
//7,5 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[117] = 8;
//7,6 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[118] = 14;
//7,7 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[119] = 9;
//7,8 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[120] = 12;
//7,9 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[121] = 7;
//7,10 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[122] = 3;
//7,11 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[123] = 10;
//7,12 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[124] = 10;
//7,13 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[125] = 6;
//7,14 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[126] = 5;
//7,15 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[127] = 5;
//8,0 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[128] = 9;
//8,1 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[129] = 2;
//8,2 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[130] = 12;
//8,3 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[131] = 3;
//8,4 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[132] = 12;
//8,5 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[133] = 3;
//8,6 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[134] = 12;
//8,7 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[135] = 3;
//8,8 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[136] = 2;
//8,9 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[137] = 10;
//8,10 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[138] = 12;
//8,11 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[139] = 9;
//8,12 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[140] = 10;
//8,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[141] = 10;
//8,14 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[142] = 6;
//8,15 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[143] = 5;
//9,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[144] = 3;
//9,1 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[145] = 12;
//9,2 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[146] = 3;
//9,3 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[147] = 14;
//9,4 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[148] = 3;
//9,5 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[149] = 14;
//9,6 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[150] = 3;
//9,7 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[151] = 12;
//9,8 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[152] = 9;
//9,9 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[153] = 14;
//9,10 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[154] = 3;
//9,11 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[155] = 6;
//9,12 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[156] = 9;
//9,13 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[157] = 12;
//9,14 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[158] = 9;
//9,15 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[159] = 4;
//10,0 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[160] = 9;
//10,1 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[161] = 6;
//10,2 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[162] = 13;
//10,3 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[163] = 9;
//10,4 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[164] = 12;
//10,5 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[165] = 9;
//10,6 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[166] = 10;
//10,7 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[167] = 4;
//10,8 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[168] = 3;
//10,9 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[169] = 8;
//10,10 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[170] = 10;
//10,11 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[171] = 12;
//10,12 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[172] = 5;
//10,13 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[173] = 5;
//10,14 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[174] = 5;
//10,15 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[175] = 7;
//11,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[176] = 3;
//11,1 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[177] = 12;
//11,2 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[178] = 1;
//11,3 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[179] = 6;
//11,4 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[180] = 3;
//11,5 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[181] = 4;
//11,6 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[182] = 9;
//11,7 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[183] = 6;
//11,8 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[184] = 11;
//11,9 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[185] = 4;
//11,10 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[186] = 13;
//11,11 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[187] = 3;
//11,12 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[188] = 6;
//11,13 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[189] = 3;
//11,14 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[190] = 2;
//11,15 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[191] = 12;
//12,0 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[192] = 13;
//12,1 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[193] = 3;
//12,2 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[194] = 6;
//12,3 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[195] = 9;
//12,4 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[196] = 12;
//12,5 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[197] = 5;
//12,6 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[198] = 5;
//12,7 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[199] = 13;
//12,8 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[200] = 9;
//12,9 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[201] = 6;
//12,10 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[202] = 3;
//12,11 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[203] = 8;
//12,12 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[204] = 14;
//12,13 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[205] = 9;
//12,14 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[206] = 10;
//12,15 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[207] = 6;
//13,0 left: 0, bottom: 0, right: 0, top: 1
obstructle_memory[208] = 1;
//13,1 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[209] = 10;
//13,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[210] = 10;
//13,3 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[211] = 6;
//13,4 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[212] = 3;
//13,5 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[213] = 6;
//13,6 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[214] = 3;
//13,7 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[215] = 4;
//13,8 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[216] = 5;
//13,9 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[217] = 9;
//13,10 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[218] = 10;
//13,11 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[219] = 6;
//13,12 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[220] = 9;
//13,13 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[221] = 6;
//13,14 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[222] = 11;
//13,15 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[223] = 12;
//14,0 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[224] = 5;
//14,1 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[225] = 9;
//14,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[226] = 10;
//14,3 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[227] = 12;
//14,4 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[228] = 9;
//14,5 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[229] = 12;
//14,6 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[230] = 9;
//14,7 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[231] = 2;
//14,8 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[232] = 6;
//14,9 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[233] = 3;
//14,10 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[234] = 10;
//14,11 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[235] = 12;
//14,12 left: 0, bottom: 1, right: 0, top: 1
obstructle_memory[236] = 5;
//14,13 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[237] = 11;
//14,14 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[238] = 8;
//14,15 left: 0, bottom: 1, right: 0, top: 0
obstructle_memory[239] = 4;
//15,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[240] = 3;
//15,1 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[241] = 2;
//15,2 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[242] = 14;
//15,3 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[243] = 3;
//15,4 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[244] = 6;
//15,5 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[245] = 7;
//15,6 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[246] = 3;
//15,7 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[247] = 10;
//15,8 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[248] = 10;
//15,9 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[249] = 10;
//15,10 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[250] = 10;
//15,11 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[251] = 6;
//15,12 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[252] = 3;
//15,13 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[253] = 10;
//15,14 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[254] = 6;
//15,15 left: 0, bottom: 1, right: 1, top: 1
obstructle_memory[255] = 7;
}
