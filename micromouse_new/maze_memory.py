from maze_loader import Maze_Obstacle_Mapper
import heapq

class Maze_Memory:
    def __init__(self, max_x, max_y, maze_file=""):
        self.max_x = max_x
        self.max_y = max_y
        self.target_x = None
        self.target_y = None
        self.path_available = 0
        self.memory = {}
        self.visited = {}
        self.destination = {}
        self.destination_point = None
        self.may_be_destination = False
        self.factor = 4 if max_x <= 16 else 8
        self.__destination_mapper = []
        self.maze_loader = Maze_Obstacle_Mapper(maze_file)
        
    def __obstacle_decoder(self, obstacle):
        array = ["left", "bottom", "right", "top"]
        temp = {}
        size = len(array)
        for index, each in enumerate(array):
            temp[each] = (obstacle & (1 << (size - index - 1)) > 0)
        return temp
    
    def process_key(self, current_x, current_y):
        value = 0
        value |= current_x
        value = value << self.factor
        value |= current_y
        return value
    
    def add_visited(self, x, y):
        self.visited[(x, y)] = True
        
    def is_visited(self, x, y):
        # print(f"x: {x}, y: {y}", (x, y) in self.visited)
        return (x, y) in self.visited
    
    def get_number_of_paths_available(self):
        return self.path_available
    
    def is_four_interconnected(self):
        missed_path = 0
        for each_node in self.__destination_mapper:
            for each_path in self.get_possible_nodes(each_node[0], each_node[1]):
                if each_path not in self.__destination_mapper:
                    missed_path += 1
        return (missed_path == 1)
    
    def check_is_valid_destination(self):
        print("Need to check this destination:", self.__destination_mapper)
        min_x, min_y = self.max_x, self.max_x
        max_x, max_y = 0, 0
        for each in self.__destination_mapper:
            min_x = min(min_x, each[0])
            min_y = min(min_y, each[1])
            max_x = max(max_x, each[0])
            max_y = max(max_y, each[1])
        if ((max_x - min_x) == 1) and ((max_y - min_y) == 1) and self.is_four_interconnected():
            print(f"Destination Found {min_x, min_y} to {max_x, max_y}")
            self.destination["min_x"] = min_x
            self.destination["min_y"] = min_y
            self.destination["max_x"] = max_x
            self.destination["max_y"] = max_y
            self.destination_point = self.__destination_mapper[1]
        self.__destination_mapper = []
        self.may_be_destination = False
        
    def get_destination(self):
        return self.destination_point
        
    def __print_debug(self, current_x, current_y):
        print(f"Destination Check {current_x}, {current_y}, self.path_available: {self.path_available}, length of destination: {len(self.__destination_mapper)}")
        
    def update_destination(self, current_x, current_y):
        if self.may_be_destination:
            if (self.path_available == 1):
                self.__destination_mapper.append((current_x, current_y))
            elif len(self.__destination_mapper) == 3 and self.path_available == 0:
                self.__destination_mapper.append((current_x, current_y))
            else:
                self.__destination_mapper = []
                self.may_be_destination = False
        if len(self.__destination_mapper) == 4:
            self.check_is_valid_destination()
        if self.path_available == 2:
            self.may_be_destination = True
            self.__destination_mapper.append((current_x, current_y))
        # if self.may_be_destination:
        #     self.__print_debug(current_x, current_y)
            
    def get_possible_nodes(self, current_x, current_y):
        if not self.is_visited(current_x, current_y):
            return []
        current_grid = self.__obstacle_decoder(self.memory[self.process_key(current_x, current_y)])
        possible_nodes = []
        if ((current_x - 1) >= 0) and (current_grid["left"] == 0):
            possible_nodes.append((current_x - 1, current_y))
        if ((current_y - 1) >= 0) and (current_grid["top"] == 0):
            possible_nodes.append((current_x, current_y - 1))
        if ((current_x + 1) <= self.max_x) and (current_grid["right"] == 0):
            possible_nodes.append((current_x + 1, current_y))
        if ((current_y + 1) <= self.max_y) and (current_grid["bottom"] == 0):
            possible_nodes.append((current_x, current_y + 1))
        return possible_nodes
        
    def get_next_path_learning(self, current_x, current_y):
        current_grid = self.__obstacle_decoder(self.maze_loader.get_obstacle((current_x, current_y)))
        self.memory[self.process_key(current_x, current_y)] = self.maze_loader.get_obstacle((current_x, current_y))
        self.path_available = 0
        next_path = None
        if ((current_x - 1) >= 0) and (current_grid["left"] == 0) and not self.is_visited(current_x - 1, current_y):
            self.path_available += 1
            next_path = (current_x - 1, current_y)
        if ((current_y - 1) >= 0) and (current_grid["top"] == 0) and not self.is_visited(current_x, current_y - 1):
            self.path_available += 1
            next_path = (current_x, current_y - 1)
        if ((current_x + 1) <= self.max_x) and (current_grid["right"] == 0) and not self.is_visited(current_x + 1, current_y):
            self.path_available += 1
            next_path = (current_x + 1, current_y)
        if ((current_y + 1) <= self.max_y) and (current_grid["bottom"] == 0) and not self.is_visited(current_x, current_y + 1):
            self.path_available += 1
            next_path = (current_x, current_y + 1)
        if not self.destination_point:
            self.update_destination(current_x, current_y)
        return next_path
    
    def astar_get_shortest(self, current_path, next_path):
        print(f"Need to travel from: {current_path} to: {next_path}")
        def heuristic(a, b):
            return abs(a[0] - b[0]) + abs(a[1] - b[1])
        
        def get_neighbours(node):
            return self.get_possible_nodes(node[0], node[1])
        
        open_set = []
        heapq.heappush(open_set, (0, current_path))
        came_from = {}
        g_score = {current_path: 0}
        f_score = {current_path: heuristic(current_path, next_path)}
        
        while open_set:
            _, current = heapq.heappop(open_set)
            if current == next_path:
                path = []
                while current in came_from:
                    path.append(current)
                    current = came_from[current]
                #path.append(start)
                path.reverse()
                return path

            for neighbor in get_neighbours(current):
                tentative_g_score = g_score[current] + 1
                if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = tentative_g_score + heuristic(neighbor, next_path)
                    if neighbor not in open_set:
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))
        return None
        
if __name__ == "__main__":
    max_x = 32
    max_y = 32
    file_to_load =  r"C:\D_Drive\Events\MicroMouse\maze_download_32x32\20241004110712.json"
    maze_mem_obj = Maze_Memory(max_x, max_y, file_to_load)
    # maze_mem_obj.add_visited(2, 5)
    print(maze_mem_obj.get_next_path_learning(1, 1))
    print(maze_mem_obj.get_number_of_paths_available())