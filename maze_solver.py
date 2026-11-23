from micro_mouse import *
from maze_memory import *
import json
import math
import sys
import os

class Maze_Obstrucle_Mapper:
    def __init__(self, obstrucle_file):
        self.obstrucle_file = obstrucle_file
        self.obstrucle = self.__load_file()
    
    def __load_file(self):
        with open(self.obstrucle_file) as fptr:
            data = json.load(fptr)
            data = {tuple([int(k.split(',')[0]), int(k.split(',')[1])]) : v for k,v in data.items()}
            return data
        
    def __getitem__(self, coordinates: tuple) -> dict:
        return self.obstrucle[coordinates]
    
    def get_obstrucle(self, coordinates: tuple) -> dict:
        return self.obstrucle[coordinates]
    
    def __len__(self):
        return int(math.sqrt(len(self.obstrucle.keys())))


class SingletonMeta(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        return cls._instances[cls]

class Distance_Class(metaclass=SingletonMeta):    
    def get_heuristic_distance(self, start_cooridinates, end_coordinates):
        return abs(start_cooridinates[0] - end_coordinates[0]) + abs(start_cooridinates[1] - end_coordinates[1])
    
    def get_euclidean(self, start_cooridinates, end_coordinates):
        print(f"start_cooridinates: {start_cooridinates}, end_coordinates: {end_coordinates}")
        return math.sqrt((start_cooridinates[0] - end_coordinates[0])**2 + (start_cooridinates[1] - end_coordinates[1])**2) 
    
class Node:
    def __init(self, coordinate):
        self.coordinates = coordinates
        self.g_cost = 0  #distance from the starting node
        self.h_cost = 0  #distance from the ending node
        self.f_cost = 0  #sum of distance between g_cost and h_cost

class Maze_Solver:
    def __init__(self, x, y, maze_size, micro_mouse_obj, maze_memory_obj, obstrucle_mapper_obj):
        self.x = x
        self.y = y
        self.max_x = maze_size
        self.max_y = maze_size
        self.micro_mouse_obj = micro_mouse_obj
        self.maze_memory_obj = maze_memory_obj
        self.obstrucle_mapper_obj = obstrucle_mapper_obj
        self.branch_stack  = []
        self.next_branch_path = None
        self.target_reached = False
        self.steps = 0
        self.same_steps = 0
        
        
    def solve(self):
        self.move((self.x, self.y))
        while self.target_reached:
            paths = self.maze_memory_obj.get_next_shortest(self.x, self.y, self.obstrucle_mapper_obj[self.x, self.y])
        
    # def update_visited(self):
    #     self.maze_memory_obj.add_visited((self.x, self.y))
    
    # def get_next_branch_path(self):
    #     return self.path_stack.pop()
    #     # if type(new_branch) == type([]):
    #     #     current_path = new_branch.pop()                    
    #     #     if len(new_branch) > 0:
    #     #         self.path_stack.append(new_branch)
    #     # return current_path
    
    # def backtrack(self, current_path):
    #     while self.current_path_stack:
    #         backtrack_path = self.current_path_stack.pop()
    #         print("Backtracking.......")
    #         self.move(backtrack_path, backtracking=True)
    #         if backtrack_path == self.next_branch_path:
    #             break;
    #         elif (len(self.current_path_stack) == 0):
    #             self.current_path_stack = self.path_stack_history.pop()
    #             self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #     print(("Backtracking Complete."))
    #     return backtrack_path
        
    # def solve(self, check_visited=False):
    #     self.move((self.x, self.y))
    #     while not self.target_reached:
    #         paths = self.maze_memory_obj.get_next_shortest(self.x, self.y, self.obstrucle_mapper_obj[self.x, self.y])
    #         try:
    #             current_path = paths.pop()
    #         except IndexError:
    #             print(f"Seems Like Dead End x:{self.x}, y:{self.y}")
    #             self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack, self.max_x*self.max_y+1)
    #             self.next_branch_path = self.get_next_branch_path()
    #             current_path = self.backtrack(self.current_path_stack)
    #             #assert abs(self.x - current_path[0]) + abs(self.y - current_path[1]) == 1,  "Both X and Y Values are modified its impossible"
    #             self.current_path_stack = [(self.x, self.y)]
    #         if len(paths) > 0:
    #             # print(f"MultiBranch @ x: {self.x}, y: {self.y}")
    #             # print(self.current_path_stack)
    #             #print(current_path, paths)
    #             self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #             self.path_stack.append((self.x, self.y))
    #             self.path_stack_history.append(self.current_path_stack)
    #             self.current_path_stack = [(self.x, self.y)]
    #             # print("path_stack_history", self.path_stack_history)
    #         self.move(current_path)
    #         if self.maze_memory_obj.is_destination(self.x, self.y):
    #             self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #             while self.path_stack_history:
    #                 self.current_path_stack = self.path_stack_history.pop()
    #                 self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #             #self.maze_memory_obj.update_shortest_coordinates_calculation((self.x, self.y), is_destination=True)
    #             print("Detination Reached")
    #             print(f"Overall steps taken = {self.steps}")
    #             print(f"Overall same steps taken = {self.same_steps}")
    #             print(self.path_stack)
    #             return True
    #         # print(current_path)
    #         # print(self.path_stack)
    
    # def solve_for_reverse(self, check_visited=False):
    #     while True:
    #         if len(self.path_stack):
    #             self.next_branch_path = self.get_next_branch_path()
    #             current_path = self.backtrack(self.current_path_stack)
    #         else:
    #             break
    #         while True:
    #             paths = self.maze_memory_obj.get_next_shortest(self.x, self.y, self.obstrucle_mapper_obj[self.x, self.y])
    #             try:
    #                 current_path = paths.pop()
    #             except:
    #                 print(f"Seems Like Dead End x:{self.x}, y:{self.y}")
    #                 self.next_branch_path = self.get_next_branch_path()
    #                 current_path = self.backtrack(self.current_path_stack)
    #                 self.current_path_stack = [(self.x, self.y)]
    #             if len(paths) > 0:
    #                 # print(f"MultiBranch @ x: {self.x}, y: {self.y}")
    #                 # print(self.current_path_stack)
    #                 #print(current_path, paths)
    #                 self.path_stack.append((self.x, self.y))
    #                 self.path_stack_history.append(self.current_path_stack)
    #                 self.current_path_stack = [(self.x, self.y)]
    #             self.move(current_path)
    #             # if self.maze_memory_obj.is_destination(self.x, self.y):
    #     # self.move((self.x, self.y))
    #     # while not self.target_reached:
    #     #     paths = self.maze_memory_obj.get_next_shortest(self.x, self.y, self.obstrucle_mapper_obj[self.x, self.y])
    #     #     try:
    #     #         current_path = paths.pop()
    #     #     except IndexError:
    #     #         print(f"Seems Like Dead End x:{self.x}, y:{self.y}")
    #     #         self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack, self.max_x*self.max_y+1)
    #     #         self.next_branch_path = self.get_next_branch_path()
    #     #         current_path = self.backtrack(self.current_path_stack)
    #     #         #assert abs(self.x - current_path[0]) + abs(self.y - current_path[1]) == 1,  "Both X and Y Values are modified its impossible"
    #     #         self.current_path_stack = [(self.x, self.y)]
    #     #     if len(paths) > 0:
    #     #         # print(f"MultiBranch @ x: {self.x}, y: {self.y}")
    #     #         # print(self.current_path_stack)
    #     #         #print(current_path, paths)
    #     #         self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #     #         self.path_stack.append((self.x, self.y))
    #     #         self.path_stack_history.append(self.current_path_stack)
    #     #         self.current_path_stack = [(self.x, self.y)]
    #     #         # print("path_stack_history", self.path_stack_history)
    #     #     self.move(current_path)
    #     #     if self.maze_memory_obj.is_destination(self.x, self.y):
    #     #         self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #     #         while self.path_stack_history:
    #     #             self.current_path_stack = self.path_stack_history.pop()
    #     #             self.maze_memory_obj.update_shortest_coordinates_calculation(self.current_path_stack)
    #     #         #self.maze_memory_obj.update_shortest_coordinates_calculation((self.x, self.y), is_destination=True)
    #     #         print("Detination Reached")
    #     #         print(f"Overall steps taken = {self.steps}")
    #     #         print(f"Overall same steps taken = {self.same_steps}")
    #     #         print(self.path_stack)
    #     #         return True
    
    # def move(self, coordinates, backtracking=False):
    #     if not((self.x == coordinates[0]) and (self.y == coordinates[1])):
    #         self.steps += 1
    #     else:
    #         self.same_steps += 1
    #     self.x = coordinates[0]
    #     self.y = coordinates[1]
    #     if not backtracking:
    #         self.current_path_stack.append((self.x, self.y))
    #     print(f"Moving x: {self.x}, y: {self.y}")
    #     self.update_visited()
    
    # def solve_move(self, coordinates):
    #     self.x = coordinates[0]
    #     self.y = coordinates[1]
    #     print(f"Moving x: {self.x}, y: {self.y}")
    
    # def solve_using_learning(self, start_x, start_y):
    #     print("*"*25)
    #     print("*"*2 + " Solving from Learning")
    #     print("*"*25)
    #     path = self.maze_memory_obj.get_next_shortest_from_learning((start_x, start_y))
    #     for each in path:
    #         self.solve_move(each)
    #     print("Memory Solve Steps: ", len(path))
    #     return True
    
if __name__ == "__main__":
    # start_x = 0
    # start_y = 0
    # min_coordinates = (7, 7)
    # max_coordinates = (8, 8)
    # # dir_path = r"C:\D_Drive\MicroMouse\maze_download"
    # # files = [os.path.join(dir_path, files) for files in os.listdir(dir_path) if files.endswith(".json")]
    # # print(f"Number of Mazes: {len(files)}")
    # # for file_name in files:
    # file_name = r"C:\D_Drive\Events\MicroMouse\MazeSimulator\dist\Generated\20241001120913.json"
    # mom = Maze_Obstrucle_Mapper(file_name)
    # maze_size = len(mom)
    # micro_mouse_obj = MicroMouse(start_x, start_y, maze_size, maze_size)
    # maze_memory_obj = MazeMemory(maze_size, maze_size, min_coordinates, max_coordinates)
    # ms_obj = Maze_Solver(start_x, start_y, maze_size, micro_mouse_obj, maze_memory_obj, mom)
    # solved = ms_obj.solve()
    # if solved:
    #     print(f"Solved: {file_name}")
    # else:
    #     print(f"Failed to solve: {file_name}")
    #     sys.exit(1)
    # maze_memory_obj.backtrack_to_start_coordinates()
    # solved = ms_obj.solve(True)
    # if solved:
    #     print(f"Backtracked: {file_name}")
    # else:
    #     print(f"Failed to Backtracked: {file_name}")
    #     sys.exit(1)
    # if solved:
    #     print(f"Solved with shortest path: {file_name}")
    # else:
    #     print(f"Failed to solve with shortest path: {file_name}")
    #     sys.exit(1)
    # shortest_path = ms_obj.solve_using_learning(start_x, start_y)
    distance_obj = Distance_Class()
    current_coordinates = (2,0)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (3,0)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (2,1)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (1,1)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (3,1)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (13, 10)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (14, 11)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (13, 9)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))
    current_coordinates = (13, 11)
    end_coordinates = (7,7)
    print(distance_obj.get_euclidean(current_coordinates, end_coordinates))