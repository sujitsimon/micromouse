#include <map>
#include <tuple>
#include <string>
#include <cstdint>

std::map<std::tuple<uint8_t, uint8_t>, > obstructle_memory;
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
