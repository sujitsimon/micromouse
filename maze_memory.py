
import time
class MazeMemory:
    def __init__(self, row, column, min_coordinates, max_coordinates):
        self.row = row
        self.column = column
        self.memory = {} #[[0]*column for i in range(row)]
        self.min_coordinates = min_coordinates
        self.max_coordinates = max_coordinates
        self.create_initial_memory_new()
        self.visited = set()
        self.obstructle_memory = {}
        self.strictly_prohibited_memory = []
        self.__update_strictly_prohibited()
        
    def __update_strictly_prohibited(self):
        start_row = int(self.row / 2) - 1
        end_row = int(self.row / 2) +1
        start_column = int(self.column / 2) - 1
        end_column = int(self.column / 2) + 1
        for i in range(start_row, end_row):
            for j in range(start_column, end_column):
                self.strictly_prohibited_memory.append((i,j))
        
    def create_initial_memory(self):
        assert self.row == self.column, "Only Square Matrix Supported"
        max_value = self.row - 2
        for i in range(int(self.row/2)):
            for j in range(int(self.column/2)):
                # self.memory[i][j] = max_value - i - j
                # self.memory[self.row - i - 1][j] = max_value - i - j
                # self.memory[i][self.column - j - 1] = max_value - i - j
                # self.memory[self.row - i - 1][self.column - j - 1] = max_value - i - j
                self.memory[(i,j)] = max_value - i - j
                self.memory[(self.row - i - 1, j)] = max_value - i - j
                self.memory[(i, self.column - j - 1)] = max_value - i - j
                self.memory[(self.row - i - 1, self.column - j - 1)] = max_value - i - j
    
    
    def get_next_shortest(self, x, y, current_grid, skip_visited=False):
        # print(x, y, current_grid)
        self.obstructle_memory[(x,y)] = current_grid
        minimum_path = {}
        #left
        if ((x - 1) >= 0) and (current_grid["left"] == 0):
            if (skip_visited or ((x-1, y) not in self.visited)):
                minimum_path[(x-1, y)] = self.memory[(x-1, y)]
        #right
        if ((x + 1) < self.row) and (current_grid["right"] == 0):
            if (skip_visited or ((x+1, y) not in self.visited)):
                minimum_path[(x+1, y)] = self.memory[(x+1, y)]
        #top
        if ((y - 1) >= 0) and (current_grid["top"] == 0):
            if (skip_visited or ((x, y - 1) not in self.visited)):
                minimum_path[(x, y-1)] = self.memory[(x, y-1)]
        #bottom
        if ((y + 1) < self.column) and (current_grid["bottom"] == 0):
            if (skip_visited or ((x, y + 1) not in self.visited)):
                minimum_path[(x, y+1)] = self.memory[(x, y+1)]
        minimum_path = dict(sorted(minimum_path.items(), key=lambda x:x[1], reverse=True))
        return list(minimum_path.keys())
    
    def is_destination(self, x, y):
        return self.memory[(x,y)] == 0
        
    def add_visited(self, coordinates):
        self.visited.add(coordinates)
    
    def check_if_visited(self, coordinates):
        return coordinates in self.visited
    
    def get_next_shortest_from_learning(self, coordinates):
        path = []
        while True:
            value = self.memory[coordinates]
            path.append(coordinates)
            if value == 0:
                break
            if (coordinates[0]-1, coordinates[1]) in self.memory and (value - self.memory[(coordinates[0]-1, coordinates[1])]) == 1 and self.check_if_visited((coordinates[0]-1, coordinates[1])):
                coordinates = (coordinates[0]-1, coordinates[1])
                continue
            if (coordinates[0]+1, coordinates[1]) in self.memory and (value - self.memory[(coordinates[0]+1, coordinates[1])]) == 1 and self.check_if_visited((coordinates[0]+1, coordinates[1])):
                coordinates = (coordinates[0]+1, coordinates[1])
                continue
            if (coordinates[0], coordinates[1]-1) in self.memory and (value - self.memory[(coordinates[0], coordinates[1]-1)]) == 1 and self.check_if_visited((coordinates[0], coordinates[1]-1)):
                coordinates = (coordinates[0], coordinates[1]-1)
                continue
            if (coordinates[0], coordinates[1]+1) in self.memory and (value - self.memory[(coordinates[0], coordinates[1]+1)]) == 1 and self.check_if_visited((coordinates[0], coordinates[1]+1)):
                coordinates = (coordinates[0], coordinates[1]+1)
                continue
            print(len(path))
        return path
    
    def backtrack_to_start_coordinates(self):
        min_coordinates = (0,0)
        max_coordinates = (0,0)
        self.create_initial_memory_new()
    
    def create_initial_memory_new(self):
        minumm_coorinates = min(self.min_coordinates)
        value = minumm_coorinates * 2 + abs(self.min_coordinates[1] - self.min_coordinates[0])
        # if (self.min_coordinates[1] > self.min_coordinates[0]):
        #     value -= (self.min_coordinates[1] + self.min_coordinates[0] + 1)
        row_sign = -1
        column_sign = -1
        for i in range(0, self.row):
            if i > 0:
                value = self.memory[(i-1, 0)] + row_sign
            for j in range(0, self.column):
                self.memory[(i, j)] = value
                if j == self.min_coordinates[1]:
                    column_sign = 0
                if j == self.max_coordinates[1]:
                    column_sign = 1
                value += column_sign
            column_sign = -1
            if i == self.min_coordinates[0]:
                row_sign = 0
            if i == self.max_coordinates[0]:
                row_sign = 1
        # for i in range(0, self.row):
        #     for i in range(0, self.column):
        #         self.memory[(i, j)] = self.memory[(i, j)] - abs(self.min_coordinates[0] - self.min_coordinates[1])

    def update_min_max_coordinates_to_start(self):
        self.min_coordinates = (0,0)
        self.max_coordinates = (0,0)
    #check it later
    # def check_possible(self, current_x, current_y, next_x, next_y):
    #     # print("Next is: ", (next_x, next_y))
    #     # print("Current is: ", (current_x, current_y))
    #     if (next_x, next_y) in self.strictly_prohibited_memory:
    #         return False
    #     if (next_x >= self.row) or (next_x < 0):
    #         return False
    #     if (next_y >= self.column) or (next_y < 0):
    #         return False
    #     if (current_x, current_y) in self.obstructle_memory:
    #         # print(self.obstructle_memory[(current_x, current_y)])
    #         if ((next_x - current_x) == 1) and (self.obstructle_memory[(current_x, current_y)]['right'] == 1):
    #                 # print("Returning False")
    #                 return False
    #         if ((next_x - current_x) == -1) and (self.obstructle_memory[(current_x, current_y)]['left'] == 1):
    #                 # print("Returning False")
    #                 return False
    #         if ((next_y - current_y) == 1) and (self.obstructle_memory[(current_x, current_y)]['bottom'] == 1):
    #                 # print("Returning False")
    #                 return False
    #         if ((next_y - current_y) == -1) and (self.obstructle_memory[(current_x, current_y)]['top'] == 1):
    #                 # print("Returning False")
    #                 return False
    #     else:
    #         return False
    #     # print("Returning True")
    #     return True
            
    # def update_shortest_coordinates_calculation(self, start_coordinates, is_destination=False):
    #     start_x, start_y = start_coordinates
    #     visited = []
    #     def flood_fill(x, y, visited):
    #         if ((x,y) in visited):
    #             return
    #         print((x,y))
    #         visited.append((x,y))
    #         current_value = self.memory[(x,y)]
    #         if not(x-1 < 0):
    #             self.memory[(x-1, y)] = current_value + 1 if self.check_possible(x, y, x-1, y) else self.memory[(x-1, y)]
    #         if not(x+1 >= self.row):
    #             self.memory[(x+1, y)] = current_value + 1 if self.check_possible(x, y, x+1, y) else self.memory[(x+1, y)]
    #         if not(y-1 < 0):
    #             self.memory[(x, y-1)] = current_value + 1 if self.check_possible(x, y, x, y-1) else self.memory[(x, y-1)]
    #         if not(y+1 >= self.column):
    #             self.memory[(x, y+1)] = current_value + 1 if self.check_possible(x, y, x, y+1) else self.memory[(x, y+1)]
    #         if not(x+1 >= self.row):
    #             flood_fill(x+1, y, visited)
    #         if not(x-1 < 0):
    #             flood_fill(x-1, y, visited)
    #         if not(y+1 >= self.column):
    #             flood_fill(x, y+1, visited)
    #         if not(y-1 < 0):
    #             flood_fill(x, y-1, visited)
            
    #     flood_fill(start_x, start_y, visited)
    
    def update_shortest_coordinates_calculation(self, path, start_value=None):
        start_value = start_value
        temp_path = path.copy()
        last_x = None
        last_y = None
        while temp_path:
            coordinates = temp_path.pop()
            if (last_x, last_y) == coordinates:
                continue
            if start_value == None:
                start_value = self.memory[coordinates]
            else:
                start_value += 1
                # print(f"Updating {coordinates} with value {start_value}")
                self.memory[coordinates] = start_value
            last_x = coordinates[0]
            last_y = coordinates[1]
        
    def visualize_memory(self):
        mem_ = [[0]*self.column for i in range(self.row)]
        for i in range(0, self.row):
            for j in range(0, self.column):
                mem_[i][j] = str(self.memory[(j,i)])
        return mem_
    
if __name__ == "__main__":
    maze_mem_obj = MazeMemory(16, 16, min_coordinates, max_coordinates)
    memory = maze_mem_obj.visualize_memory()
    with open("test.csv", "w") as fptr:
        for each in memory:
            fptr.write(",".join(each) + "\n")
    # maze_mem_obj.obstructle_memory = {(0, 0): {'top': 1, 'bottom': 1, 'left': 1, 'right': 0},
    #  (0, 1): {'top': 1, 'bottom': 0, 'left': 1, 'right': 0},
    #  (0, 2): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (0, 3): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (0, 4): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (0, 5): {'top': 0, 'bottom': 1, 'left': 1, 'right': 1},
    #  (1, 0): {'top': 1, 'bottom': 0, 'left': 0, 'right': 1},
    #  (1, 1): {'top': 0, 'bottom': 0, 'left': 0, 'right': 0},
    #  (1, 2): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (1, 3): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (1, 4): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (1, 5): {'top': 0, 'bottom': 1, 'left': 1, 'right': 0},
    #  (2, 0): {'top': 0, 'bottom': 0, 'left': 1, 'right': 0},
    #  (2, 1): {'top': 0, 'bottom': 1, 'left': 0, 'right': 1},
    #  (2, 2): {'top': 1, 'bottom': 0, 'left': 1, 'right': 0},
    #  (2, 3): {'top': 0, 'bottom': 1, 'left': 1, 'right': 0},
    #  (2, 4): {'top': 1, 'bottom': 0, 'left': 1, 'right': 0},
    #  (2, 5): {'top': 0, 'bottom': 1, 'left': 0, 'right': 1},
    #  (3, 0): {'top': 1, 'bottom': 0, 'left': 0, 'right': 1},
    #  (3, 1): {'top': 0, 'bottom': 1, 'left': 1, 'right': 0},
    #  (3, 2): {'top': 1, 'bottom': 0, 'left': 0, 'right': 1},
    #  (3, 3): {'top': 0, 'bottom': 0, 'left': 0, 'right': 1},
    #  (3, 4): {'top': 0, 'bottom': 1, 'left': 0, 'right': 1},
    #  (3, 5): {'top': 1, 'bottom': 1, 'left': 1, 'right': 0},
    #  (4, 0): {'top': 1, 'bottom': 0, 'left': 1, 'right': 0},
    #  (4, 1): {'top': 0, 'bottom': 1, 'left': 0, 'right': 1},
    #  (4, 2): {'top': 1, 'bottom': 0, 'left': 1, 'right': 1},
    #  (4, 3): {'top': 0, 'bottom': 1, 'left': 1, 'right': 0},
    #  (4, 4): {'top': 1, 'bottom': 0, 'left': 1, 'right': 0},
    #  (4, 5): {'top': 0, 'bottom': 1, 'left': 0, 'right': 1},
    #  (5, 0): {'top': 1, 'bottom': 0, 'left': 0, 'right': 1},
    #  (5, 1): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (5, 2): {'top': 0, 'bottom': 0, 'left': 1, 'right': 1},
    #  (5, 3): {'top': 0, 'bottom': 0, 'left': 0, 'right': 1},
    #  (5, 4): {'top': 0, 'bottom': 0, 'left': 0, 'right': 1},
    #  (5, 5): {'top': 0, 'bottom': 1, 'left': 1, 'right': 1}}
    # maze_mem_obj.update_shortest_coordinates_calculation((3, 3))
    # mem = maze_mem_obj.visualize_memory()
    # sp = maze_mem_obj.strictly_prohibited_memory
    
            
                