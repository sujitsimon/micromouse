import json
import math
import sys
import os

class Maze_Obstacle_Mapper:
    def __init__(self, obstacle_file):
        self.obstacle_file = obstacle_file
        self.factor = 4 
        self.obstacle = self.__load_file()
    
    def __load_file(self):
        with open(self.obstacle_file) as fptr:
            data = json.load(fptr)
            self.factor = 4 if (len(data.keys()) < 16 * 16 ) else 8
            data = {self.process_key(k) : self.process_value(v) for k,v in data.items()}
            return data
        
    def process_key(self, key):
        value = 0
        x, y = int(key.split(",")[0]), int(key.split(",")[1])
        value |= x
        value = value << self.factor
        value |= y
        return value
        
    def process_value(self, value):
        bin_value = 0
        array = ["left", "bottom", "right", "top"]
        for index, each in enumerate(array, 1):
            bin_value |=  value[each]
            if index != 4:
                bin_value = bin_value<< 1
        return bin_value
        
    def __getitem__(self, coordinates: tuple) -> dict:
        final_coordinates = 0
        final_coordinates |= coordinates[0]
        final_coordinates = final_coordinates << self.factor
        final_coordinates |= coordinates[1]
        return self.obstacle[final_coordinates]
    
    def get_obstacle(self, coordinates: tuple) -> dict:
        final_coordinates = 0
        final_coordinates |= coordinates[0]
        final_coordinates = final_coordinates << self.factor
        final_coordinates |= coordinates[1]
        return self.obstacle[final_coordinates]
    
    def __len__(self):
        return int(math.sqrt(len(self.obstrucle.keys())))
    
    def obstacle_decoder(self, obstacle):
        array = ["left", "bottom", "right", "top"]
        temp = {}
        size = len(array)
        for index, each in enumerate(array):
            temp[each] = (obstacle & (1 << (size - index - 1)) > 0)
        return temp

if __name__ == "__main__":
    obstacle_file =  r"C:\\D_Drive\\Events\\MicroMouse\\maze_download_32x32\\20241004110712.json"
    mapper = Maze_Obstacle_Mapper(obstacle_file)
    print(mapper.obstacle_decoder(mapper[(1, 1)]))