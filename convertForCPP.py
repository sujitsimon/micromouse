import json
HEADER = """#include <map>
#include <tuple>
#include <string>
#include <cstdint>

"""

mapper = {
    1: "true",
    0: "false"
    }

def process_key(key = "1,0"):
    value = 0
    x, y = int(key.split(",")[0]), int(key.split(",")[1])
    value |= x
    value = value << 8
    value |= y
    return str(value)
process_key()
    

content = HEADER
file_path = r"C:\D_Drive\Events\MicroMouse\MazeSimulator\dist\Generated\20241001120913.json"
with open (file_path) as fptr:
    data = json.load(fptr)
content += "std::map<std::tuple<uint8_t, uint8_t>, > obstructle_memory;\n"
length = len(data.items())
i = 1
for key, value in data.items():
    bin_value = 0
    array = ["left", "bottom", "right", "top"]
    for index, each in enumerate(array, 1):
        bin_value |=  value[each]
        if index != 4:
            bin_value = bin_value<< 1;
    string = f'//{key} left: {value["left"]}, bottom: {value["bottom"]}, right: {value["right"]}, top: {value["top"]}\n'
    string += 'memory['+process_key(key)+'] = '+str(bin_value) + ';\n'
    i+=1
    content += string
with open("test_1.cpp", "w") as fptr:
    fptr.write(content)