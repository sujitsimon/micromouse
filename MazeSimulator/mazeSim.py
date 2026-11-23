# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mazeSim.ui'
#
# Created by: PyQt5 UI code generator 5.12.3
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def __init__(self):
        print("Ui Main Window init")
        
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(930, 764)
        MainWindow.setTabShape(QtWidgets.QTabWidget.Rounded)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.groupBox = QtWidgets.QGroupBox(self.centralwidget)
        self.groupBox.setGeometry(QtCore.QRect(720, 540, 181, 181))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBox.sizePolicy().hasHeightForWidth())
        self.groupBox.setSizePolicy(sizePolicy)
        self.groupBox.setObjectName("groupBox")
        self.gridLayoutWidget = QtWidgets.QWidget(self.groupBox)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(10, 20, 158, 141))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.clearButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        self.clearButton.setObjectName("clearButton")
        self.gridLayout.addWidget(self.clearButton, 3, 2, 1, 1)
        self.label_2 = QtWidgets.QLabel(self.gridLayoutWidget)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 1, 1, 1, 1)
        self.fillButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        self.fillButton.setObjectName("fillButton")
        self.gridLayout.addWidget(self.fillButton, 3, 1, 1, 1)
        self.loadButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        self.loadButton.setObjectName("loadButton")
        self.gridLayout.addWidget(self.loadButton, 4, 1, 1, 1)
        self.exportButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        self.exportButton.setObjectName("exportButton")
        self.gridLayout.addWidget(self.exportButton, 4, 2, 1, 1)
        self.widthBox = QtWidgets.QSpinBox(self.gridLayoutWidget)
        self.widthBox.setMinimum(1)
        self.widthBox.setMaximum(100)
        self.widthBox.setValue(6)
        self.widthBox.setObjectName("widthBox")
        self.gridLayout.addWidget(self.widthBox, 0, 2, 1, 1)
        self.heightBox = QtWidgets.QSpinBox(self.gridLayoutWidget)
        self.heightBox.setMaximum(100)
        self.heightBox.setMinimum(1)
        self.heightBox.setValue(6)
        self.heightBox.setObjectName("heightBox")
        self.gridLayout.addWidget(self.heightBox, 1, 2, 1, 1)
        self.label = QtWidgets.QLabel(self.gridLayoutWidget)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 1, 1, 1)
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setGeometry(QtCore.QRect(0, 0, 1121, 721))
        self.frame.setCursor(QtGui.QCursor(QtCore.Qt.ArrowCursor))
        self.frame.setAutoFillBackground(False)
        self.frame.setStyleSheet("background-color : #fff")
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setLineWidth(10)
        self.frame.setMidLineWidth(50)
        self.frame.setObjectName("frame")
#         self.line = QtWidgets.QFrame(self.frame)
#         self.line.setGeometry(QtCore.QRect(20, 20, 10, 10))
#         sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
#         sizePolicy.setHorizontalStretch(0)
#         sizePolicy.setVerticalStretch(0)
#         sizePolicy.setHeightForWidth(self.line.sizePolicy().hasHeightForWidth())
#         self.line.setSizePolicy(sizePolicy)
#         self.line.setStyleSheet("border: 5px solid black;\n""border-radius: 5px;")
#         self.line.setFrameShadow(QtWidgets.QFrame.Plain)
#         self.line.setLineWidth(50)
#         self.line.setMidLineWidth(10)
#         self.line.setFrameShape(QtWidgets.QFrame.HLine)
#         self.line.setObjectName("line")
#         self.line_2 = QtWidgets.QFrame(self.frame)
#         self.line_2.setGeometry(QtCore.QRect(23, 23, 50, 5))
#         self.line_2.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
#         self.line_2.setStyleSheet("color: #C7C7C7\n"
# "\n"
# "\n"
# "")
#         self.line_2.setFrameShadow(QtWidgets.QFrame.Plain)
#         self.line_2.setLineWidth(5)
#         self.line_2.setFrameShape(QtWidgets.QFrame.HLine)
#         self.line_2.setObjectName("line_2")
#         self.line_2.raise_()
#         self.line.raise_()
        self.frame.raise_()
        self.groupBox.raise_()
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1118, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.groupBox.setTitle(_translate("MainWindow", "Configuration"))
        self.clearButton.setText(_translate("MainWindow", "CLEAR"))
        self.label_2.setText(_translate("MainWindow", "Height:"))
        self.fillButton.setText(_translate("MainWindow", "FILL"))
        self.loadButton.setText(_translate("MainWindow", "LOAD"))
        self.exportButton.setText(_translate("MainWindow", "EXPORT"))
        self.label.setText(_translate("MainWindow", "Width:"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
