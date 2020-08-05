import os
from shutil import copyfile
from datetime import datetime

#First, the required information will be prepared

conceptsNum = 9
learningPhasesAggreg = 3
aggregPhasesName = ["WS", "R1", "R2"]
learningPhasesDisagg = 6

path = "/home/gjaimovitch/tfg/MHexperiment/"

witnessSets = [[('0', '0')],[('10', '0')],[('', '0'), ('01', '')],[('0', '011')],[('', '1'), ('1110', '')],[('0', '10'), ('00', '')],[('', '0'), ('0', ''), ('00', '0')],[('', '01'), ('01', ''), ('1', '')],[('', '010'), ('0', ''), ('1', '')]]
teachingSets1 = [[('111001', '1'), ('110101', '1')],[('01000', '1'), ('01010', '1')],[('010100', ''), ('10101', '1')],[('10', '1'), ('001', '011')],[('10', '1'), ('111001', '')],[('11', '01'), ('10', '')],[('0001', '0'), ('01', '1')],[('11', ''), ('011', '')],[('100', ''), ('110', '')]]
teachingSets2 = [[('100110', '1'), ('111100', '1'), ('01010', '0')],[('001', '0'), ('00', '0'), ('1011', '0')],[('010', ''), ('100', '1'), ('011101', '')],[('0001', '011'), ('00', '011'), ('000', '011')],[('11', '1'), ('0001', '0'), ('11100', '')],[('', '1'), ('101', ''), ('000', '')],[('0101', '1'), ('0010', '0'), ('0110', '1')],[('10', ''), ('0100', ''), ('0', '0')],[('10', ''), ('101', ''), ('11', '')]]
unlabeledSets = [[('001110', '0'), ('101', '1'), ('101101', '1'), ('01', '0'), ('01000', '0')],[('11001', '1'), ('101110', '0'), ('00011', '0'), ('100110', '0'), ('00000', '0')],[('011111', ''), ('0110', ''), ('010000', ''), ('010110', ''), ('01111', '')],[('1', '1'), ('0000', '011'), ('11', '1'), ('0010', '011'), ('0011', '011')],[('01101', ''), ('1001', '0'), ('111000', ''), ('1', '1'), ('01', '1')],[('0000', ''), ('100', ''), ('1', '11'), ('01', '00'), ('0001', '')],[('0111', '1'), ('000', '0'), ('0000', '0'), ('10', ''), ('011', '1')],[('010', ''), ('0110', ''), ('0101', ''), ('101', ''), ('0111', '')],[('010', ''), ('000', ''), ('01', ''), ('011', ''), ('00', '')]]


#The directory MHexperiment keeps 9 folders (1, 2, ..., 9) where the files required for launching MagicHaskeller will be created
def createFiles():
  for i in range(0, conceptsNum):
    f = open(path + str(i + 1) + "/in_MH_WS.txt", 'a')
    for k in range(0, len(witnessSets[i])):
      f.write("f " + '"' + witnessSets[i][k][0] + '"' + " == " + '"' + witnessSets[i][k][1] + '"' + " ")
      if k < (len(witnessSets[i])-1):
        f.write("&& ")
    f.close()

    copyfile(path + str(i + 1) + "/in_MH_WS.txt", path + str(i + 1) + "/in_MH_R1.txt")

    f = open(path + str(i + 1) + "/in_MH_R1.txt", 'a')
    f.write("&& ")
    for k in range(0, len(teachingSets1[i])):
      f.write("f " + '"' + teachingSets1[i][k][0] + '"' + " == " + '"' + teachingSets1[i][k][1] + '"' + " ")
      if k < (len(teachingSets1[i])-1):
        f.write("&& ")
    f.close()

    copyfile(path + str(i + 1) + "/in_MH_R1.txt", path + str(i + 1) + "/in_MH_R2.txt")


    f = open(path + str(i + 1) + "/in_MH_R2.txt", 'a')
    f.write("&& ")
    for k in range(0, len(teachingSets2[i])):
      f.write("f " + '"' + teachingSets2[i][k][0] + '"' + " == " + '"' + teachingSets2[i][k][1] + '"' + " ")
      if k < (len(teachingSets2[i])-1):
        f.write("&& ")
    f.close()


    f = open(path + str(i + 1) + "/out_MH_WS.txt", 'x')
    f = open(path + str(i + 1) + "/out_MH_R1.txt", 'x')
    f = open(path + str(i + 1) + "/out_MH_R2.txt", 'x')
    f = open(path + str(i + 1) + "/log_WS.txt", 'x')
    f = open(path + str(i + 1) + "/log_R1.txt", 'x')
    f = open(path + str(i + 1) + "/log_R2.txt", 'x')


def runMHexperiment():
  for i in range(0,conceptsNum):
    for j in range(0, learningPhasesAggreg):
      query = "/home/gjaimovitch/.cabal/bin/MagicHaskeller -i < /home/gjaimovitch/tfg/MHexperiment/" + str(i + 1) + "/in_MH_" + aggregPhasesName[j] + ".txt 1> /home/gjaimovitch/tfg/MHexperiment/" + str(i + 1) + "/out_MH_" + aggregPhasesName[j] + ".txt 2> /home/gjaimovitch/tfg/MHexperiment/" + str(i + 1) + "/log_" + aggregPhasesName[j] + ".txt"
      print(query)
      print(datetime.now().strftime("%H:%M:%S"))
      os.system(query)