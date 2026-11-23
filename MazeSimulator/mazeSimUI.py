from mazeSim import Ui_MainWindow
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QFileDialog
import json
import math
import sys
import os
from datetime import datetime

sys.path.insert(0, "../")
from maze_gen import *

CORNER_SPACE = 40
LINE_HEIGHT = 40
lineDirections = ["top", "left", "bottom", "right"]


class Dot:
    def __init__(self, frame, coordinates):
        # print(f"Creating Dot {coordinates}")
        self.x = coordinates[0]
        self.y = coordinates[1]
        self.line = QtWidgets.QFrame(frame)
        self.line.setGeometry(QtCore.QRect(CORNER_SPACE * (self.x + 1), CORNER_SPACE * (self.y + 1), 5, 5))
        self.line.setStyleSheet("border: 5px solid black;\n""border-radius: 5px;")
        self.line.setFrameShadow(QtWidgets.QFrame.Plain)
        self.line.raise_()
        self.line.show()
        
    def getDot(self):
        return self.line
    
    def deleteDot(self):
        self.line.setParent(None)
        self.line.deleteLater()
        # print(f"Dot at ({self.x}, {self.y}) deleted")
        
class Line(QtCore.QObject):
    def __init__(self, frame, coordinates, active, side):
        # print(f"Creating Dot {coordinates}")
        super().__init__()
        self.x = coordinates[0]
        self.y = coordinates[1]
        self.line = QtWidgets.QFrame(frame)
        self.active = active
        self.side = side
        if side == "top":
            self.line.setGeometry(QtCore.QRect(CORNER_SPACE * (self.x + 1), CORNER_SPACE * (self.y + 1), LINE_HEIGHT, 5))
        elif side == "bottom":
            self.line.setGeometry(QtCore.QRect(CORNER_SPACE * (self.x + 1), CORNER_SPACE * (self.y + 2), LINE_HEIGHT, 5))
        elif side == "left":
            self.line.setGeometry(QtCore.QRect(CORNER_SPACE * (self.x + 1), CORNER_SPACE * (self.y + 1), 5, LINE_HEIGHT))
        elif side == "right":
            self.line.setGeometry(QtCore.QRect(CORNER_SPACE * (self.x + 2), CORNER_SPACE * (self.y + 1), 5, LINE_HEIGHT))
        self.line.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        if active:
            self.line.setStyleSheet("color: #000\n")
        else:
            self.line.setStyleSheet("color: #E7E7E7\n")
        self.line.setFrameShadow(QtWidgets.QFrame.Plain)
        self.line.setLineWidth(5)
        if side == "top" or side == "bottom":
            self.line.setFrameShape(QtWidgets.QFrame.HLine)
        else:
            self.line.setFrameShape(QtWidgets.QFrame.VLine)
        # self.line.raise_()
        self.line.installEventFilter(self)
        self.line.show()
        
    def getLine(self):
        return self.line
    
    def isWall(self):
        if self.active:
            return 1
        else:
            return 0
    
    def toggleActive(self):
        self.active = not self.active
        if self.active:
            self.line.setStyleSheet("color: #000\n")
        else:
            self.line.setStyleSheet("color: #E7E7E7\n")
    
    def deleteLine(self):
        self.line.setParent(None)
        self.line.deleteLater()
        
    def eventFilter(self, source, event):
        if event.type() == QtCore.QEvent.MouseButtonPress and source is self.line:
            if event.button() == QtCore.Qt.LeftButton:
                self.onLineClick()
                self.isDragging = True
                return True
        elif event.type() == QtCore.QEvent.MouseMove and self.isDragging:
            self.onLineDrag(event)
        elif event.type() == QtCore.QEvent.MouseButtonRelease and self.isDragging:
            if event.button() == QtCore.Qt.LeftButton:
                self.isDragging = False
                return True
        return super().eventFilter(source, event)
    
    def onLineDrag(self, event):
        print(f"Dot at ({self.x}, {self.y}) {self.side} Dragged {event} {event.x()} {event.y()}")        

    def onLineClick(self):
        print(f"Dot at ({self.x}, {self.y}) {self.side} clicked")
        self.toggleActive()
        # print(f"Dot at ({self.x}, {self.y}) deleted")
        
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

class Ui_Functional(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(Ui_Functional, self).__init__()
        self.setupUi(self)
        self.widthBox.valueChanged.connect(self.widthChange)
        self.heightBox.valueChanged.connect(self.heightChange)
        self.width = self.widthBox.value()
        self.height = self.heightBox.value()
        self.mazeContent = {}
        self.mazeDot = {}
        self.mazeLine = {}
        self.fillButton.clicked.connect(self.fillMaze)
        self.clearButton.clicked.connect(self.clearMaze)
        self.loadButton.clicked.connect(self.loadJsonFile)
        self.exportButton.clicked.connect(self.exportMaze)
        self.mom_obj = None
        self.isDragging = False
        self.draw_maze()
        
    def widthChange(self, value):
        print(f"Width changed to: {value}")
        self.width = value
        self.draw_maze()

    def heightChange(self, value):
        print(f"Height changed to: {value}")
        self.height = value
        self.draw_maze()
        
    def draw_maze(self, fill_maze = False, useJson = False):
        for each in self.mazeDot:
            try:
                self.mazeDot[each].deleteDot()
            except:
                pass
        for each in self.mazeLine:
            try:
                self.mazeLine[each].deleteLine()
            except:
                pass
        for i in range(0, self.height + 1):
            for j in range(0, self.width + 1):
                for directions in lineDirections:
                    # if not(directions == "right"):
                    #     continue
                    if (i == self.height) or (j == self.width):
                        continue
                    if (j, i) not in self.mazeLine:
                        self.mazeLine[(j, i)] = {}
                    if (j > 0) and directions == "left":
                        self.mazeLine[(j, i)]["left"] = self.mazeLine[(j - 1, i)]["right"]
                    elif (i > 0) and directions == "top":
                        self.mazeLine[(j, i)]["top"] = self.mazeLine[(j, i-1)]["bottom"]
                    elif (i == 0) and directions == "top":
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), True, directions)
                    elif (i == self.height - 1) and directions == "bottom":
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), True, directions)
                    elif (j == 0) and directions == "left":
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), True, directions)
                    elif (j == self.width - 1) and directions == "right":
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), True, directions)
                    elif useJson:
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), self.mom_obj.get_obstrucle((j, i))[directions], directions)
                    else:
                        self.mazeLine[(j, i)][directions] = Line(self.frame, (j, i), fill_maze, directions)
                self.mazeDot[(j, i)] = Dot(self.frame, (j, i))
                
    def fillMaze(self):
        print("Fill Maze Clicked")
        self.draw_maze(True)
    
    def clearMaze(self):
        print("Clear Maze Clicked")
        self.draw_maze(False)
        
    def loadJsonFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(self,"Select Maze JSON file", "","JSON Files (*.json)", options=options)
        if fileName:
            self.mom_obj = Maze_Obstrucle_Mapper(fileName)
            self.widthBox.setValue(len(self.mom_obj))
            self.heightBox.setValue(len(self.mom_obj))
            self.draw_maze(fill_maze = False, useJson = True)
    
    def exportMaze(self):
        maze = {}
        for each in self.mazeLine:
            for each_direction in lineDirections:
                if each not in maze:
                    maze[each] = {}
                maze[each][each_direction] = self.mazeLine[each][each_direction].isWall()
        datetime_obj = datetime.now()
        timestamp_c = datetime_obj.strftime("%Y%m%d%H%M%S")
        file_name = os.path.join(os.getcwd(), "Generated", f"{timestamp_c}.json")
        maze_gen_obj = MazeGenerator(file_name, True)
        # maze_gen_obj.min_x1 = 0
        # maze_gen_obj.min_x2 = 0
        # maze_gen_obj.min_y1 = 0
        # maze_gen_obj.min_y2 = 0
        # maze_gen_obj.max_x1 = self.widthBox.value()
        # maze_gen_obj.max_x2 = self.widthBox.value()
        # maze_gen_obj.max_y1 = self.heightBox.value()
        # maze_gen_obj.max_y2 = self.heightBox.value()
        maze_gen_obj.maze = maze
        maze_gen_obj.draw_maze_from_drawing(maze, file_name=file_name.replace(".json", ".png"),
                               save=True, mark_coordinates=True,
                               width = self.widthBox.value(), height = self.heightBox.value())
        data = {f"{key[0]},{key[1]}": value for key, value in maze.items()}
        with open(file_name, "w") as fptr:
            json.dump(data, fptr, indent=4)
            
            
            
    
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = Ui_Functional()
    MainWindow.show()
    sys.exit(app.exec_())