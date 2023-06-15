#!/usr/bin/python3
import sys
import os


def writeto_file(name_node_ip_path, matrixA, matrixB, m, p):
    new_ip_file = open(name_node_ip_path, "w+")
    for row_idx, row in enumerate(matrixA):
        new_ip_file.write("A " + str(row_idx) + " " + str(p) + " " + str(','.join([str(ele) for ele in row])) + "\n")
    for row_idx, row in enumerate(matrixB):
        new_ip_file.write("B " + str(row_idx) + " " + str(m) + " " + str(','.join([str(ele) for ele in row])) + "\n")    
    new_ip_file.close()

def getMatrix(line, file):
    mat = []
    for i in range(line[0]):
        mat.append([int(ele) for ele in file.readline().strip().split()])
    return mat


def update_input(name_node_ip_path):
    file = open(name_node_ip_path, "r")
    line = [int(ele) for ele in file.readline().strip().split()]
    m = line[0]
    matrixA = getMatrix(line, file)
    line = [int(ele) for ele in file.readline().strip().split()]
    p = line[1]
    matrixB = getMatrix(line, file)
    file.close()
    writeto_file(name_node_ip_path, matrixA, matrixB, m, p)


path_hadoop = sys.argv[1] # /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar
name_node_ip_path = sys.argv[2]# /Q3final/input.txt

hdfs_inputPath = sys.argv[3]+'/input.txt' # /input/input.txt
output_file_dfs = sys.argv[4] # /tmp/output311
mapperPath = sys.argv[5]+'/mapper.py' # /Q3final/mapper.py
reducerPath = sys.argv[5]+'/reducer.py' # /Q3final/reducer.py

update_input(name_node_ip_path)


os.system('hdfs dfs -rm -r '+hdfs_inputPath)
os.system('hdfs dfs -rm -r '+output_file_dfs)
os.system('hdfs dfs -mkdir '+hdfs_inputPath)
os.system('hdfs dfs -put '+name_node_ip_path+' '+hdfs_inputPath)



args = sys.argv[7:]



command = 'hadoop jar '+path_hadoop+' '+'-file '+mapperPath+' '+'-mapper "python3 mapper.py '+' '.join(args)+'" '+'-file '+reducerPath+' '+'-reducer "python3 reducer.py" '+'-input '+hdfs_inputPath+' '+'-output '+output_file_dfs

os.system(command)


# hadoop jar /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar -file /Q3final/mapper.py -mapper "python3 mapper.py" -file /Q3final/reducer.py -reducer "python3 reducer.py" -input /input/modified_input.txt -output /tmp/output3111



# ./runner.py /input/input.txt /Q2final1/input.txt /tmp/output311 /Q2final1/mapper.py /Q2final1/reducer.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar


# ./runner_script.py /input /Q1final1 /tmp/output311 /Q1final1 /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar




# ./runner_script.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar /Q3final /input /tmp/output311 /Q3final

# ./runner_script.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar /Q2final2/input.txt /input /tmp/output311 /Q2final2

