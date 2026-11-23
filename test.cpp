#include <map>
#include <tuple>
#include <string>
#include <cstdint>

std::map<std::tuple<uint8_t, uint8_t>, > obstructle_memory;
//0,0 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[0] = 13;
//0,1 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[1] = 9;
//0,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[2] = 10;
//0,3 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[3] = 10;
//0,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[4] = 10;
//0,5 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[5] = 14;
//1,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[16] = 3;
//1,1 left: 0, bottom: 0, right: 0, top: 0
obstructle_memory[17] = 0;
//1,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[18] = 10;
//1,3 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[19] = 10;
//1,4 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[20] = 10;
//1,5 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[21] = 12;
//2,0 left: 1, bottom: 0, right: 0, top: 0
obstructle_memory[32] = 8;
//2,1 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[33] = 6;
//2,2 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[34] = 9;
//2,3 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[35] = 12;
//2,4 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[36] = 9;
//2,5 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[37] = 6;
//3,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[48] = 3;
//3,1 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[49] = 12;
//3,2 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[50] = 3;
//3,3 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[51] = 2;
//3,4 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[52] = 6;
//3,5 left: 1, bottom: 1, right: 0, top: 1
obstructle_memory[53] = 13;
//4,0 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[64] = 9;
//4,1 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[65] = 6;
//4,2 left: 1, bottom: 0, right: 1, top: 1
obstructle_memory[66] = 11;
//4,3 left: 1, bottom: 1, right: 0, top: 0
obstructle_memory[67] = 12;
//4,4 left: 1, bottom: 0, right: 0, top: 1
obstructle_memory[68] = 9;
//4,5 left: 0, bottom: 1, right: 1, top: 0
obstructle_memory[69] = 6;
//5,0 left: 0, bottom: 0, right: 1, top: 1
obstructle_memory[80] = 3;
//5,1 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[81] = 10;
//5,2 left: 1, bottom: 0, right: 1, top: 0
obstructle_memory[82] = 10;
//5,3 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[83] = 2;
//5,4 left: 0, bottom: 0, right: 1, top: 0
obstructle_memory[84] = 2;
//5,5 left: 1, bottom: 1, right: 1, top: 0
obstructle_memory[85] = 14;
