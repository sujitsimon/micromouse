import xml.etree.ElementTree as ET
import math
import tkinter
import json
#from tkinter import Image
import matplotlib.pyplot as plt
# from PIL import Image, ImageDraw
import os
from multiprocessing import Pool
import ipycanvas as ipc
from PIL import ImageGrab
import turtle

WIDTH = 16
HEIGHT = 16
MAZE_OFFSET = 2

FONT_SIZE_DEFAULT = 8

FONT_SIZE = {
            6: FONT_SIZE_DEFAULT,
            16: 3,
            32: 2.3
             }

class SVG_Parser:
    def __init__(self, file_name):
        self.file_name = file_name
        self.tree = ET.parse(file_name)
        self.root = self.tree.getroot()
    
    def get_line(self):
        for line in self.root.findall("./{http://www.w3.org/2000/svg}g/{http://www.w3.org/2000/svg}line"):
            yield line
            
class MazeGenerator:
    def __init__(self, file_name, skip_svg = False):
        self.file_name = file_name
        if not skip_svg:
            self.svg_parser_obj = SVG_Parser(file_name)
        self.min_x1 = math.inf
        self.min_x2 = math.inf
        self.min_y1 = math.inf
        self.min_y2 = math.inf
        self.max_x1 = 0
        self.max_x2 = 0
        self.max_y1 = 0
        self.max_y2 = 0
        self.maze = {}
        self.maze_sheet = {}
        
    def get_boundaries(self):
        for line in self.svg_parser_obj.get_line():
            x1 = int(line.attrib['x1'])
            x2 = int(line.attrib['x2'])
            y1 = int(line.attrib['y1'])
            y2 = int(line.attrib['y2'])
            
            self.min_x1 = min(self.min_x1, x1)
            self.min_x2 = min(self.min_x2, x2)
            self.min_y1 = min(self.min_y1, y1)
            self.min_y2 = min(self.min_y2, y2)

            self.max_x1 = max(self.max_x1, x1)
            self.max_x2 = max(self.max_x2, x2)
            self.max_y1 = max(self.max_y1, y1)
            self.max_y2 = max(self.max_y2, y2)
        
    def get_maze_grid_dimension(self):
        x_min = min(self.min_x1, self.min_x2)
        x_max = max(self.max_x1, self.max_x2)
        y_min = min(self.min_y1, self.min_y2)
        y_max = max(self.max_y1, self.max_y2)
        return int((x_max - x_min) / WIDTH), int((y_max - y_min) / HEIGHT)
    
    def get_line_direction(self, x1, x2, y1, y2):
        if x1 == x2:
            return 'vertical'
        return 'horizondal'
    
    def is_left_or_right(self, x0, x1):
        if (x0 * WIDTH + MAZE_OFFSET) == x1:
            return 'right'
        return 'left'
    
    def is_top_or_bottom(self, y0, y1):
        if (y0 * HEIGHT + MAZE_OFFSET) == y1:
            return 'bottom'
        return 'top'    
    
    def get_number_of_blocks(self, start, end):
        return int((end - start)/WIDTH)
    
    def get_start_block(self, x1, y1):
        x1, y1 = math.ceil(x1/WIDTH), math.ceil(y1/HEIGHT)
        return (x1, y1)
    
    def toggle_position(self, position):
        if "left":
            return "right"
        if "right":
            return "left"
        if "top":
            return "bottom"
        if "bottom":
            return "top"
    
    # working
    # def generate_maze(self):
    #     x, y = self.get_maze_grid_dimension()
    #     for i in range(1, x+1):
    #         for j in range(1, y+1):
    #             # 0 means open, 1 means block or wall
    #             self.maze[(i,j)] = {
    #                                 "top": 0,
    #                                 "bottom": 0,
    #                                 "left": 0,
    #                                 "right": 0,
    #                                }
    #     for index, line in enumerate(self.svg_parser_obj.get_line(), 1):
    #         x1 = int(line.attrib['x1'])
    #         x2 = int(line.attrib['x2'])
    #         y1 = int(line.attrib['y1'])
    #         y2 = int(line.attrib['y2'])
    #         line_direction = self.get_line_direction(x1, x2, y1, y2)
    #         if line_direction == 'vertical':
    #             number_of_blocks = self.get_number_of_blocks(y1, y2)
    #         else:
    #             number_of_blocks = self.get_number_of_blocks(x1, x2)
    #         (x0, y0) = self.get_start_block(x1, y1)
    #         for i in range(0, number_of_blocks):
    #             if line_direction == 'horizondal': #should modify top and bottom
    #                 x0 += i
    #                 position = self.is_top_or_bottom(y0, y1)
    #                 if (x0 <= x) and (y0 <= y):
    #                     self.maze[(x0, y0)][position] = 1
    #                 if position == "top" and ((y0 - 1) > 0) and (x0 <= x):
    #                     position = 'bottom'
    #                     self.maze[(x0, (y0 - 1))][position] = 1
    #                 elif position == "bottom" and ((y0 + 1) <= y) and (x0 <= x):
    #                     position = 'top'
    #                     self.maze[(x0, (y0 + 1))][position] = 1
    #                 x0 -= i
    #             if line_direction == 'vertical': #should modify left and right
    #                 y0 += i
    #                 position = self.is_left_or_right(x0, x1)
    #                 if (y0 <= y) and (x0 <= x):
    #                     self.maze[(x0, y0)][position] = 1
    #                 if position == "left" and ((x0 - 1) > 0) and (y0 <= y):
    #                     position = 'right'
    #                     self.maze[((x0 - 1), y0)][position] = 1
    #                 elif position == "right" and ((x0 + 1) <= x) and (y0 <= y):
    #                     position = 'left'
    #                     self.maze[((x0 + 1), y0)][position] = 1
    #                 y0 -= i
    #     return self.maze
    
    def generate_maze(self):
        x, y = self.get_maze_grid_dimension()
        for i in range(1, x+1):
            for j in range(1, y+1):
                # 0 means open, 1 means block or wall
                self.maze[(i,j)] = {
                                    "top": 0,
                                    "bottom": 0,
                                    "left": 0,
                                    "right": 0,
                                   }
        for index, line in enumerate(self.svg_parser_obj.get_line(), 1):
            x1 = int(line.attrib['x1'])
            x2 = int(line.attrib['x2'])
            y1 = int(line.attrib['y1'])
            y2 = int(line.attrib['y2'])
            line_direction = self.get_line_direction(x1, x2, y1, y2)
            if line_direction == 'vertical':
                number_of_blocks = self.get_number_of_blocks(y1, y2)
            else:
                number_of_blocks = self.get_number_of_blocks(x1, x2)
            (x0, y0) = self.get_start_block(x1, y1)
            for i in range(0, number_of_blocks):
                if line_direction == 'horizondal': #should modify top and bottom
                    x0 += i
                    position = self.is_top_or_bottom(y0, y1)
                    if (x0 <= x) and (y0 <= y):
                        self.maze[(x0, y0)][position] = 1
                    if position == "top" and ((y0 - 1) > 0) and (x0 <= x):
                        position = 'bottom'
                        self.maze[(x0, (y0 - 1))][position] = 1
                    elif position == "bottom" and ((y0 + 1) <= y) and (x0 <= x):
                        position = 'top'
                        self.maze[(x0, (y0 + 1))][position] = 1
                    x0 -= i
                if line_direction == 'vertical': #should modify left and right
                    y0 += i
                    position = self.is_left_or_right(x0, x1)
                    if (y0 <= y) and (x0 <= x):
                        self.maze[(x0, y0)][position] = 1
                    if position == "left" and ((x0 - 1) > 0) and (y0 <= y):
                        position = 'right'
                        self.maze[((x0 - 1), y0)][position] = 1
                    elif position == "right" and ((x0 + 1) <= x) and (y0 <= y):
                        position = 'left'
                        self.maze[((x0 + 1), y0)][position] = 1
                    y0 -= i
        return self.maze
    
    def get_center(self, x1, y1, x2, y2):
        return (int((x1+x2)/2), int((y1+y2)/2))
    
    
    def draw_maze(self, maze, file_name = None, save=False, mark_coordinates=False, width = None, height = None):
        temp_width = 18
        temp_height = 18
        if (width == None) or (height == None):    
            w, h = self.get_maze_grid_dimension()
        else:
            w = width
            h = height
        w = (w + 2) * temp_width
        h = (h + 2) * temp_height

        fig, ax = plt.subplots()
        ax.set_xlim(0, w)
        ax.set_ylim(h, 0)
        ax.axis('off')
        ax.set_aspect('equal')

        for each_coordinate in self.maze:
            (x, y) = each_coordinate
            if maze[each_coordinate]['top'] == 1:
                ax.plot([x * temp_width, (x + 1) * temp_width], [y * temp_height, y * temp_height], color='black')
            if maze[each_coordinate]['bottom'] == 1:
                ax.plot([x * temp_width, (x + 1) * temp_width], [(y + 1) * temp_height, (y + 1) * temp_height], color='black')
            if maze[each_coordinate]['left'] == 1:
                ax.plot([x * temp_width, x * temp_width], [y * temp_height, (y + 1) * temp_height], color='black')
            if maze[each_coordinate]['right'] == 1:
                ax.plot([(x + 1) * temp_width, (x + 1) * temp_width], [y * temp_height, (y + 1) * temp_height], color='black')
            if mark_coordinates:
                center_x, center_y = self.get_center((x)*temp_width, (y)*temp_height, (x+1)*temp_width, (y+1)*temp_height)
                if (width == None) or (height == None):    
                    xmax, ymax = self.get_maze_grid_dimension()
                else:
                    xmax, ymax = width + 1, height + 1
                fontsize = FONT_SIZE[xmax] if xmax in FONT_SIZE else FONT_SIZE_DEFAULT
                ax.text(center_x, center_y, f"{x-1},{y-1}", fontsize=fontsize, ha='center', va='center')
        if save:
            if file_name == None:
                file_name = self.file_name
            print("Saving File:", file_name)
            plt.savefig(file_name.replace(".svg", ".png"), dpi=300, pad_inches=0)
        else:
            plt.show()
        plt.close()
    
    def draw_maze_from_drawing(self, maze, file_name = None, save=False, mark_coordinates=False, width = None, height = None):
        temp_width = 18
        temp_height = 18
        if (width == None) or (height == None):    
            w, h = self.get_maze_grid_dimension()
        else:
            w = width
            h = height
        w = (w + 2) * temp_width
        h = (h + 2) * temp_height

        fig, ax = plt.subplots()
        ax.set_xlim(0, w)
        ax.set_ylim(h, 0)
        ax.axis('off')
        ax.set_aspect('equal')

        for each_coordinate in self.maze:
            (x, y) = each_coordinate
            if maze[each_coordinate]['top'] == 1:
                ax.plot([x * temp_width, (x + 1) * temp_width], [y * temp_height, y * temp_height], color='black')
            if maze[each_coordinate]['bottom'] == 1:
                ax.plot([x * temp_width, (x + 1) * temp_width], [(y + 1) * temp_height, (y + 1) * temp_height], color='black')
            if maze[each_coordinate]['left'] == 1:
                ax.plot([x * temp_width, x * temp_width], [y * temp_height, (y + 1) * temp_height], color='black')
            if maze[each_coordinate]['right'] == 1:
                ax.plot([(x + 1) * temp_width, (x + 1) * temp_width], [y * temp_height, (y + 1) * temp_height], color='black')
            if mark_coordinates:
                center_x, center_y = self.get_center((x)*temp_width, (y)*temp_height, (x+1)*temp_width, (y+1)*temp_height)
                if (width == None) or (height == None):    
                    xmax, ymax = self.get_maze_grid_dimension()
                else:
                    xmax, ymax = width, height
                fontsize = FONT_SIZE[xmax] if xmax in FONT_SIZE else FONT_SIZE_DEFAULT
                ax.text(center_x, center_y, f"{x},{y}", fontsize=fontsize, ha='center', va='center')
        if save:
            if file_name == None:
                file_name = self.file_name
            print("Saving File:", file_name)
            plt.savefig(file_name.replace(".svg", ".png"), dpi=300, pad_inches=0)
        else:
            plt.show()
        plt.close()
            
    def gen_maze_solver_sheet(self):
        x, y = self.get_maze_grid_dimension()
        for i in range(1, x+1):
            for j in range(1, y+1):
                # 0 means open, 1 means block or wall
                self.maze_sheet[(i,j)] = {
                                    "top": 1,
                                    "bottom": 1,
                                    "left": 1,
                                    "right": 1,
                                   }
        self.draw_maze(self.maze_sheet, 'maze_solver_sheet', True, mark_coordinates=True)
    
    def draw_maze_tk(self, display = True):
        temp_width = 189
        temp_height = 189
        window = tkinter.Tk()
        w,h = self.get_maze_grid_dimension()
        w = (w + 2) * temp_width
        h = (h + 2) * temp_height
        window.geometry(f"{w}x{h}")
        self.canva = tkinter.Canvas(window, width=w, height=h, bg="white")
        for each_coordinate in self.maze:
            (x, y) = each_coordinate
            if self.maze[each_coordinate]['top'] == 1:
                self.canva.create_line(x * temp_width, y * temp_height, (x + 1) * temp_width, y * temp_height)
            if self.maze[each_coordinate]['bottom'] == 1:
                self.canva.create_line(x * temp_width, (y + 1) * temp_height, (x + 1) * temp_width, (y + 1) * temp_height)
            if self.maze[each_coordinate]['left'] == 1:
                self.canva.create_line(x * temp_width, y * temp_height, x * temp_width, (y + 1) * temp_height)
            if self.maze[each_coordinate]['right'] == 1:
                self.canva.create_line((x + 1) * temp_width, y * temp_height, (x + 1) * temp_width, (y + 1) * temp_height)
        self.canva.pack()
        if display:
            window.mainloop()
    

    def draw_maze_ipycanvas(self, display=True):
        temp_width = 189
        temp_height = 189
        w, h = self.get_maze_grid_dimension()
        w = (w + 2) * temp_width
        h = (h + 2) * temp_height
        self.canvas = ipc.Canvas(width=w, height=h)
    
        for each_coordinate in self.maze:
            (x, y) = each_coordinate
            if self.maze[each_coordinate]['top'] == 1:
                self.canvas.stroke_line(x * temp_width, y * temp_height, (x + 1) * temp_width, y * temp_height)
            if self.maze[each_coordinate]['bottom'] == 1:
                self.canvas.stroke_line(x * temp_width, (y + 1) * temp_height, (x + 1) * temp_width, (y + 1) * temp_height)
            if self.maze[each_coordinate]['left'] == 1:
                self.canvas.stroke_line(x * temp_width, y * temp_height, x * temp_width, (y + 1) * temp_height)
            if self.maze[each_coordinate]['right'] == 1:
                self.canvas.stroke_line((x + 1) * temp_width, y * temp_height, (x + 1) * temp_width, (y + 1) * temp_height)
    
        self.canvas.to_file('canvas_image.png', sync_image_data=True)
    
    def draw_maze_turtle(self, display=True):
        temp_width = 30
        temp_height = 30
        window = turtle.Screen()
        window.bgcolor("white")
        w, h = self.get_maze_grid_dimension()
        w = (w + 2) * temp_width
        h = (h + 2) * temp_height
        window.setup(width=w, height=h)
        t = turtle.Turtle()
        t.speed(0)  # Set the fastest drawing speed

        for each_coordinate in self.maze:
            (x, y) = each_coordinate
            t.penup()
            if self.maze[each_coordinate]['top'] == 1:
                t.goto(x * temp_width, y * temp_height)
                t.pendown()
                t.setheading(0)
                t.forward(temp_width)
                t.penup()
            if self.maze[each_coordinate]['bottom'] == 1:
                t.goto(x * temp_width, (y + 1) * temp_height)
                t.pendown()
                t.setheading(0)
                t.forward(temp_width)
                t.penup()
            if self.maze[each_coordinate]['left'] == 1:
                t.goto(x * temp_width, y * temp_height)
                t.pendown()
                t.setheading(90)
                t.forward(temp_height)
                t.penup()
            if self.maze[each_coordinate]['right'] == 1:
                t.goto((x + 1) * temp_width, y * temp_height)
                t.pendown()
                t.setheading(90)
                t.forward(temp_height)
                t.penup()

        if display:
            window.mainloop()
    # def save_image(self):
    #     self.draw_maze(display=False)
    #     self.canva.postscript(self.file_name.replace(".svg", ".eps"))
    #     img = Image.open(self.file_name.replace(".svg", ".eps"))
    #     img.save(self.file_name.replace(".svg", ".png"), 'png')
        
    def save_maze_json(self):
        data = {f"{key[0]-1},{key[1]-1}": value for key, value in self.maze.items()}
        with open(self.file_name.replace(".svg", ".json"), "w") as fptr:
            json.dump(data, fptr, indent=4)

def solve_for_file(file_name):
    print(file_name)
    maze_gen_obj = MazeGenerator(file_name)
    maze_gen_obj.get_boundaries()
    x,y = maze_gen_obj.get_maze_grid_dimension()
    maze = maze_gen_obj.generate_maze()
    maze_gen_obj.draw_maze(maze, save=True, mark_coordinates=True)
    maze_gen_obj.gen_maze_solver_sheet()
    maze_gen_obj.save_maze_json()
    #maze_gen_obj.draw_maze_turtle()



if __name__ == "__main__":
    directory = r"C:\D_Drive\Events\MicroMouse\maze_download_32x32"
    files_list = os.listdir(directory)
    files = [os.path.join(directory, files) for files in files_list if files.endswith(".svg") and files.replace(".svg", ".json") not in files_list]
    with Pool(processes=25) as pool:
        pool.map(solve_for_file, files)
        
    # for file_name in files:
    #     print(file_name)
    #     maze_gen_obj = MazeGenerator(file_name)
    #     maze_gen_obj.get_boundaries()
    #     x,y = maze_gen_obj.get_maze_grid_dimension()
    #     maze = maze_gen_obj.generate_maze()
    #     maze_gen_obj.draw_maze(maze, save=True, mark_coordinates=True)
    #     maze_gen_obj.gen_maze_solver_sheet()
    #     maze_gen_obj.save_maze_json()
        # break