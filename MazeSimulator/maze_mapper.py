import json
import re

LOGS_PATH = r"C:\D_Drive\Events\MicroMouse\teraterm\teraterm.log"
REGEX = r"^\[.*\]\sprintDebugMemory\sx:\s([\d]+)\sy:\s([\d]+)\sValue:\s([\d]+)$"


with open(LOGS_PATH, "r") as fptr:
    lines = fptr.readlines()


obstacle = {}




def parse_obstacle(value):
    value = int(value)
    obst = {
        "top": 0,
        "left": 0,
        "bottom": 0,
        "right": 0
        }
    obst["top"] = (value & (1<<0)) >> 0
    obst["right"] = (value & (1<<1)) >> 1
    obst["bottom"] = (value & (1<<2)) >> 2
    obst["left"] = (value & (1<<3)) >> 3
    return obst



for i in range(16):
    for j in range(16):
        obstacle[f"{i},{j}"] = parse_obstacle(15)
        
for line in lines:
    if match := re.match(REGEX, line, re.I|re.M):
        x = match.group(1)
        y = match.group(2)
        obs = parse_obstacle(match.group(3))
        obstacle[f"{x},{y}"] = obs
        print(f"{x},{y}")
        print(obs)
        
with open("maze.json", "w") as fptr:
    json.dump(obstacle, fptr, indent=4)