#!/usr/bin/python3
import sys


ans = 0
i_idx = 0
j_idx = 0
k_idx = 0
lines = []


def compute(i, k, val):
    global ans, i_idx, k_idx
    if (i, k) != (i_idx, k_idx):
        print(ans, end=" ")
        if i != i_idx:
            print()
        ans, i_idx, k_idx = val, i, k
    else:
        ans += val
    return ans


for line in sys.stdin:
    lines.append(line)
    line_len = len(lines)
    if line_len >= 2:
        line = lines[0].strip().split()
        i, k, pos = map(int, line[0].strip("()").split(","))
        val1 = int(line[1])
        line = lines[1].strip().split()
        i, k, pos = map(int, line[0].strip("()").split(","))
        val2 = int(line[1])
        val = val1 * val2
        ans = compute(i,k,val)
        lines = []
    else:
        continue

print(ans)

