# importing libraries
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import sys

# window class
class Window(QMainWindow):
	def __init__(self):
		super().__init__()
		self.setWindowTitle("Maze Drawer")
		self.setGeometry(250, 250, 1000, 1000)
		self.image = QImage(self.size(), QImage.Format_RGB32)
		self.image.fill(Qt.white)
        


if __name__ == "__main__":
    # create pyqt5 app
    App = QApplication(sys.argv)
    # create the instance of our Window
    window = Window()
    # showing the window
    window.show()
    # start the app
    sys.exit(App.exec())
