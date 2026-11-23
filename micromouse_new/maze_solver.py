from maze_memory import *
import sys
import os

class Maze_Solver:
    def __init__(self, start_x, start_y, max_x, max_y, maze_file=""):
        self.x = start_x
        self.y = start_y
        self.maze_memory = Maze_Memory(max_x, max_y, maze_file)
        self.is_maze_solved = False
        self.branch = [(0,0)]
        self.steps = 0
        
    def solve(self):
        self.move((self.x, self.y))
        while not self.is_maze_solved:
            self.maze_memory.add_visited(self.x, self.y)
            next_path = self.maze_memory.get_next_path_learning(self.x, self.y)
            if self.maze_memory.get_number_of_paths_available() > 1:
                print(f"Adding x: {self.x}, y: {self.y} to branch......")
                self.branch.append((self.x, self.y))
            if self.update_maze_solved(next_path):
                break
            if self.is_backtracking_required(next_path):
                self.backtrack()
                continue
            else:
                self.move(next_path)
        print(f"Overall Steps Taken for maze is {self.steps}")
            
    def move(self, next_path):
        self.x = next_path[0]
        self.y = next_path[1]
        if not self.is_maze_solved:
            self.steps += 1
        print(f"Moving to x: {self.x} y: {self.y}")
        
    def is_backtracking_required(self, next_path):
        if next_path == None and self.branch:
            return True
        return False
    
    def backtrack(self):
        last_branch = self.branch.pop()
        current_path = (self.x, self.y)
        path = self.maze_memory.astar_get_shortest(current_path, last_branch)
        while path:
            current_backtrack = path.pop(0)
            self.move(current_backtrack)
            
    def solve_shortest(self, start_location, end_location):
        print("Shortest Solving: Path")
        path = self.maze_memory.astar_get_shortest(start_location, end_location)
        steps_taken = len(path)
        self.move((self.x, self.y))
        while path:
            current_backtrack = path.pop(0)
            self.move(current_backtrack)
        print(f"Shortest Path Taken in {steps_taken} steps")
        
    def solve_to_destination(self, start_location):
        destination = self.maze_memory.get_destination()
        print(f"Shortest Solving: Path to destination {destination}")
        path = self.maze_memory.astar_get_shortest(start_location, destination)
        steps_taken = len(path)
        self.move((self.x, self.y))
        while path:
            current_backtrack = path.pop(0)
            self.move(current_backtrack)
        print(f"Shortest Path Taken in {steps_taken} steps")
        
    def update_maze_solved(self, next_path):
        if (next_path == None) and self.x == 0 and self.y == 0:
            self.is_maze_solved = True
            return True
        return False
    
    def checked_boxes(self):
        return len(self.maze_memory.memory.keys())
        

if __name__ == "__main__":
    start_x = 0
    start_y = 0
    max_x = 32
    max_y = 32
    directory = "C:\D_Drive\Events\MicroMouse\maze_download_32x32"
    files = [os.path.join(directory, each_file) for each_file in os.listdir(directory) if each_file.endswith(".json")]
    file_to_load =  r"C:\D_Drive\Events\MicroMouse\maze_download_32x32\20241004110712.json"
    for index, each_file in enumerate(files, 1):
        print(f"{index} Solving {each_file}")
        ms_solver_object = Maze_Solver(start_x, start_y, max_x, max_y, each_file)
        ms_solver_object.solve()
        ms_solver_object.solve_to_destination((start_x, start_y))
        print("***************End*****************")
        # ms_solver_object.solve_shortest((0,0), (8,7))
        # print("Number of boxes checked", ms_solver_object.checked_boxes())
    file_to_load = r"C:\D_Drive\Events\MicroMouse\maze_download_32x32\20241004105343.json"
    print(f"Solving {file_to_load}")
    ms_solver_object = Maze_Solver(start_x, start_y, max_x, max_y, file_to_load)
    ms_solver_object.solve()
    ms_solver_object.solve_to_destination((start_x, start_y))
    print("***************End*****************")