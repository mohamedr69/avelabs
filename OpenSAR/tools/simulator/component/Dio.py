from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import UserString
import socket
import traceback
import errno

__all__ = ['DioWidget']


#
cDioSize = 8 # for easy implementation, constant the Dio Size in Port
#
iName = 0  # Port Name
iSize = 1  # the number of Dio this port has
iDir  = 2  # if corresponding bit is 0 then it is Input, else it is output
        # IO direction can be changed by AUTOSAR Client
iLevel = 3 # current Input/Output State
DioList = [
    ['A',cDioSize,0x00,0x00],   # this Port Id is 0
    ['B',cDioSize,0x00,0x00],   # this Port Id is 1
    ['C',cDioSize,0x00,0x00],
    ['D',cDioSize,0x00,0x00],
    ['E',cDioSize,0x00,0x00],
    ['F',cDioSize,0x00,0x00],
    ['G',cDioSize,0x00,0x00],
    ['H',cDioSize,0x00,0x00],
    ['I',cDioSize,0x00,0x00],
    ['J',cDioSize,0x00,0x00],
    ['K',cDioSize,0x00,0x00]
]

cMaxDioSize = cDioSize  # the maximum size of port in the DioList

cDioInput  = 0
cDioOutput = 1

STD_HIGH = 1
STD_LOW  = 0  # low active, that is when Output, Light On

class DioButton(QPushButton):
    Dir   = 0xFF
    Level = 0xFF
    def __init__(self,Dir,PortId,BitId):
        self.PortId = PortId
        self.BitId  = BitId
        QPushButton.__init__(self,None)
        self.setDirection(Dir)
        self.connect(self,SIGNAL('clicked()'),self.on_Dio_clicked) 
    def getDirection(self):
        return self.Dir 
    def setDirection(self,Dir):
        if(Dir == self.Dir):
            return
        if(Dir==cDioInput):
            self.Dir = Dir
            self.setFlat(False)
        elif(Dir==cDioOutput):
            self.Dir = Dir
            self.setFlat(True)
        else:
            print 'Dio: Error Set Direction from AUTOSAR Client.'
        # force Direction Change
        self.Level = 0xFF
        self.setLevel(self.Level)
        
    
    def on_Dio_clicked(self):
        if(self.Dir == cDioInput):
            self.setLevel(not self.Level)
        elif(self.Dir == cDioOutput):
            pass

    def getLevel(self):
        return self.Level
    
    def setLevel(self,Level):
        if(Level == self.Level):
            return
        if(Level == STD_HIGH):
            self.Level = Level
            if(self.Dir == cDioInput):
                self.setIcon(QIcon('./res/DI_HIGH.bmp'))
            elif(self.Dir == cDioOutput):
                self.setIcon(QIcon('./res/DO_HIGH.bmp'))
        elif(Level == STD_LOW):
            self.Level = Level
            if(self.Dir == cDioInput):
                self.setIcon(QIcon('./res/DI_LOW.bmp'))
            elif(self.Dir == cDioOutput):
                self.setIcon(QIcon('./res/DO_LOW.bmp'))
        else:
            print 'Dio: Error Set Level from AUTOSAR Client.'
        self.setText('%s'%(int(Level)))
      
class PortMatrix(QTableWidget):
    def __init__(self): 
        super(QTableWidget,self).__init__(None)
        self.initPort()
        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.settimeout(10)
            self.sock.connect(('127.0.0.1', 60000)) 
        except:
            print 'Dio: AUTOSAR Client isnot started.'
        self.sock.settimeout(0)
        self.startTimer(10)  # Time Period Must be the same with the AUTOSAR client
    
    def closeEvent(self,event):
        self.sock.close()

    def timerEvent(self, event):
        try: 
            matrix = self.sock.recv(len(DioList)*2)
            self.setMatrix(matrix)
            self.sock.send(self.getMatrix())
        except socket.error as e:
#             if e.errno == errno.WSAENOBUFS:
#                 pass
            #print traceback.format_exc()
            pass

    def getMatrix(self):
        matrix = UserString.MutableString('\0'*len(DioList)*2)
        for Index in range(0,len(DioList)):
            data = 0
            for i in range(0,cMaxDioSize):
                Dio = self.cellWidget(Index, i)
                if(Dio != None):
                    if(Dio.getLevel() == STD_HIGH):
                        data |= (1<<i)&0xFF
            matrix[Index] = '%c'%(data)
        for Index in range(0,len(DioList)):
            data = 0
            for i in range(0,cMaxDioSize):
                Dio = self.cellWidget(Index, i)
                if(Dio != None):
                    if(Dio.getDirection() == cDioOutput):
                        data |= (1<<i)&0xFF
            matrix[len(DioList)+Index] = '%c'%(data)
        return matrix.data
                                  
    def setMatrix(self,matrix):
        if(len(matrix) == len(DioList)*2):
            for Index in range(0,len(DioList)):
                for i in range(0,cMaxDioSize):
                    Dio = self.cellWidget(Index, i)
                    if(Dio != None):
                        # update direction
                        if( (ord(matrix[len(DioList)+Index])&(1<<i)) != 0 ):
                            Dio.setDirection(cDioOutput)
                        else:
                            Dio.setDirection(cDioInput)
                        # update level
                        if(Dio.getDirection() == cDioOutput):
                            if( (ord(matrix[Index])&(1<<i)) != 0 ):
                                Dio.setLevel(STD_HIGH)
                            else:
                                Dio.setLevel(STD_LOW)
                    else:
                        print 'Dio: this cell is NULL.'
        else:
            print 'Dio: Error Matrix from AUTOSAR Client.',len(matrix)

    def initPort(self):
        # init Port Container
        self.setColumnCount(cMaxDioSize)
        self.setRowCount(len(DioList))
        list = []
        for i in range(0,cMaxDioSize):
            list.append('%s'%(i))
            self.setColumnWidth(i,50)
        self.setHorizontalHeaderLabels(QStringList(list))
        list = []
        for port in DioList:
            list.append(port[iName])
        self.setVerticalHeaderLabels(QStringList(list)) 
        #
        Index = 0
        for port in DioList:
            for i in range(0,port[iSize]):
                if( (port[iDir]&(1<<i)) != 0 ):
                    Dio = DioButton(cDioOutput,Index,i)
                else:
                    Dio = DioButton(cDioInput,Index,i)
                if( (port[iLevel]&(1<<i)) != 0 ):
                    Dio.setLevel(STD_HIGH)
                else:
                    Dio.setLevel(STD_LOW)
                self.setCellWidget(Index,i,Dio)
            Index += 1

class DioWidget(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowIcon(QtGui.QIcon("./res/Dio.bmp"))
        self.setWindowTitle("Dio Simulator")
        self.setCentralWidget(PortMatrix())