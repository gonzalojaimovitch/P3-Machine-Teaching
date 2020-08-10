#https://www.geeksforgeeks.org/python-find-most-frequent-element-in-a-list/ addapted

#Return the different-elements-of-a-list frequencies in a list of lists [value, frequency]
def list_frequency(passed_list):
  num = ['']     
  for i in passed_list: 
    if not [i,passed_list.count(i)] in num:
      if num[0] == '':
        num[0]= [i, passed_list.count(i)]
      else:
        num.append([i, passed_list.count(i)])

  
  return num 

witnessSets = [[('0', '0')],[('10', '0')],[('', '0'), ('01', '')],[('0', '011')],[('', '1'), ('1110', '')],[('0', '10'), ('00', '')],[('', '0'), ('0', ''), ('00', '0')],[('', '01'), ('01', ''), ('1', '')],[('', '010'), ('0', ''), ('1', '')]]
teachingSets1 = [[('111001', '1'), ('110101', '1')],[('01000', '1'), ('01010', '1')],[('010100', ''), ('10101', '1')],[('10', '1'), ('001', '011')],[('10', '1'), ('111001', '')],[('11', '01'), ('10', '')],[('0001', '0'), ('01', '1')],[('11', ''), ('011', '')],[('100', ''), ('110', '')]]
teachingSets2 = [[('100110', '1'), ('111100', '1'), ('01010', '0')],[('001', '0'), ('00', '0'), ('1011', '0')],[('010', ''), ('100', '1'), ('011101', '')],[('0001', '011'), ('00', '011'), ('000', '011')],[('11', '1'), ('0001', '0'), ('11100', '')],[('', '1'), ('101', ''), ('000', '')],[('0101', '1'), ('0010', '0'), ('0110', '1')],[('10', ''), ('0100', ''), ('0', '0')],[('10', ''), ('101', ''), ('11', '')]]

testSets = [[('00000', '0'), ('11100', '1'), ('00111', '0'), ('11010', '1'), ('0010', '0')], [('01011', '1'), ('0101', '1'), ('0010', '0'), ('100', '0'), ('1', '')], [('00010', '1'), ('110', ''), ('00111', '1'), ('11000', ''), ('101', '1')], [('01011', '011'), ('0101', '011'), ('0010', '011'), ('100', '1'), ('1', '1')], [('01011', '0'), ('110', '1'), ('0010', ''), ('101', '1'), ('1000', '')], [('01', '00'), ('0000', ''), ('00011', ''), ('0011', ''), ('1000', '')], [('01011', '1'), ('0000', '0'), ('00000', '0'), ('100', ''), ('1000', '')], [('10101', ''), ('11101', ''), ('00000', '1'), ('0011', '1'), ('1111', '')], [('10101', ''), ('0100', ''), ('00000', ''), ('01000', ''), ('1111', '')]]

#Given a list of outputs (or an output element), return the accuracy of the testList labels. The frequency of the different outputs is given, applying the
#appropriate weight for each case
def thirdAgent2(outputs, weights, testList):
  result = 0

  for j in range(0, len(testList)):
    for k in range(0, len(outputs)):
      if outputs[k] == testList[j]:
        result = result + 20 * weights[k]
      else:
        result = result + 0

  return result



numConcepts = 9
numPhases = 3

accuracyList = ['']*numConcepts

for i in range(0, numConcepts):
  accuracyList[i] = [0]*numPhases
  for i2 in range(0, numPhases):
    if i2 == 0:
      passed_list = [value[1] for value in witnessSets[i]]
    elif i2 == 1:
      passed_list = [value[1] for value in witnessSets[i]]  + [value[1] for value in teachingSets1[i]]
    else:
      passed_list = [value[1] for value in witnessSets[i]]  + [value[1] for value in teachingSets1[i]] + [value[1] for value in teachingSets2[i]]
    values = list_frequency(passed_list)
    calc_len = len(passed_list)
    print(str(i + 1) + " " + str(values))

    accuracyList[i][i2] = thirdAgent2([value[0] for value in values], [value[1]/calc_len for value in values], [value[1] for value in testSets[i]])

for i in range(0, len(accuracyList)):
  for j in range(0, len(accuracyList[i])):
    print("Concept " + str(i + 1) + " Phase " + str(j + 1) + ": " + str(accuracyList[i][j]) +  "%")