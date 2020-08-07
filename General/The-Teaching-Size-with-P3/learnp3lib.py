import ctypes

_p3 = ctypes.CDLL('/home/cesar/Dropbox/MT/P3Python/learnp3.so')
#_p3.learnp3.argtypes = (ctypes.c_char_p)
_p3.learnp3.restype = ctypes.c_char_p

def lp3(minput):
    global _p3
    result =_p3.learnp3(ctypes.c_char_p(minput.encode('utf-8')))
    #print result
    return str(result)
#gcc -o3 -fPIC -shared -o learnp3.so learnp3.c
