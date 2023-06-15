#!/usr/bin/python3
import sys

#def strfun(num):
#    return f"{num:02}"

for line in sys.stdin:
    line = line.strip().split()
    ri = int(line[1])

    ele_lis = []
    for ele in line[3].strip().split(','):
        ele_lis.append(int(ele))

    if line[0] == "A":
        for j, ele in enumerate(ele_lis):
            for k in range(int(line[2])):
               # print(f"({ri},{strfun(k)},{strfun(j)}) {ele}")
                print(str(ri).zfill(2) + ',' + str(k).zfill(2) + ',' + str(j).zfill(2) + ' ' + str(ele))
    if line[0] == "B":
        for k, ele in enumerate(ele_lis):
            for i in range(int(line[2])):
               # print(f"({strfun(i)},{strfun(k)},{ri}) {ele}")
                print(str(i).zfill(2) + ',' + str(k).zfill(2) + ',' + str(ri).zfill(2) + ' ' + str(ele))
