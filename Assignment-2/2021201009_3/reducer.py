#!/usr/bin/python3
import sys


def calculate_expectation(d):
    s = 0
    if sum(d.values()) == 0:
        return 0
    for k, v in d.items():
        s += k * v
    return s / sum(d.values())


d = dict()

for line in sys.stdin:
    line = line.strip().split()
    c = int(line[0])
    if c in d:
        d[c] += 1
    else:
        d[c] = 1

print(calculate_expectation(d))

# print("2")