
# coding: utf-8

# In[ ]:

# coding: utf-8

# In[14]:


###gcc -o3 -fPIC -shared -o learnp3.so learnp3.c


alfab=['0','1']
k=7

def add_car(string):
 res=[]
 for i in range(len(alfab)):
        res.append(string+alfab[i])
 return res      

def lex_str_size(size): 
 if (size ==0): return [""]
 if (size ==1): return alfab
 res=alfab
 resn=[]
 s=len(res[0])
 #print(s)
 while (s<size):
  resn=[]
  #print(res)  
  for elem in res:
   resn=resn+ add_car(elem)
  s=len(resn[0]) 
  #print(s)
  res=resn  
 return resn 
    
#lista=lex_str_size(8)
#print(len(lista))

### code to solve the standard Integer Partition problem 
def partition(number):
     answer = set()
     answer.add((number, ))
     for x in range(1, number):
         for y in partition(number - x):
             #answer.add((x, ) + y)
             answer.add(tuple(sorted((x, ) + y)))
     return answer


# In[15]:

def pairs_size(dsize):
  lres=[]
  for s in range(0,dsize+1):
    #print(s)
    for si in range(0,(s+1)):
       lres=lres + [ (y,x) for x in lex_str_size(si) for y in lex_str_size((s-si)) if (len(x)+ len(y)) ==dsize]
        #print (s,si)
       # lstring= lex_str_size(si)
       # for ii in lstring:
       #     for io in lex_str_size((s-si)):
       #         lres.append((ii,io))
  #len(lres)
  return lres


#Create the pairs given a size
# parts (3) =[(1, 2), (1, 1, 1), (3,), (2, 1)]

def congruent(pairs):
    for i in range(0,len(pairs)):
        (inp,out)=pairs[i]
        for j in range((i+1),len(pairs)):
            (inpn,outn)=pairs[j]
            if(inp==inpn and out!=outn): return False
    return True      
        

def emptyout(pairs):
      #check the outputs are empty
  emptyo=True
  for pair in pairs:
        if pair[1]!="": emptyo=False 
  return emptyo
    
    
import itertools
rest=[]
for i in range(0,k+1):
 parts=list(partition(i))
 for elem in parts:  
    psize=len(elem)
    #print (psize)
    pres=[]
    for x in elem:
       # print (x)
        if psize==1: pres.extend([(y,) for y in pairs_size(x)])
        else: pres.append(pairs_size(x))
    if psize==1: rest.extend(pres)
    else: rest=rest+list(itertools.product(*pres))


#print(len(parts) )	
#print(parts)
#len(rest)



        
len(rest)

#### Filters
print(len(rest))
#for cs in rest:
# print(cs) 

restf= [ tuple(set(x)) for x in rest ] #
restfo= [tuple(sorted(x)) for x in restf ] # order each ws to filter repetitions
restff=tuple(sorted(set(restfo), key=restfo.index))

rest1=[x for x in restff if congruent(x) and not emptyout(x)  ]
print(len(rest1))

#for cs in rest1:
# print(cs) 


import learnp3lib
#res=learnp3lib.lp3("(('1','10'),('0','111'))")
i=1
lprg=[]
for p in rest1:
    res=learnp3lib.lp3(str(p))
    lprg.append(res)
    print (str(i) +": " + str(p) +": "+ res + ": " +str([ len(x[0]) + len(x[1])  for x in p]))
    i=i+1

