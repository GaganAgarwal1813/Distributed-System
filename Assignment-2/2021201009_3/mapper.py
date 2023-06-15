#!/usr/bin/python3
from random import random
import sys



# def random_var_count(sum_range):
#     s = 0
#     for i in range(1,sum_range):
#         s += random()
#         if(s >= 1.0): # to check for condition
#             return i
        

for line in sys.stdin:
    line = line.strip().split()
    # sum_range = int(line[0])
    iter_count = int(line[0])
    sum_range = 100000
    ans = 0

    for i in range(iter_count):
        s = 0
        for j in range(1,sum_range):
            s += random()
            if(s >= 1.0): # to check for condition
                ans = j
                break
        print(str(ans))
    
        

