#!/usr/bin/python3
import sys
import os

# mapper reducer same path
hadoopPath = sys.argv[1] # /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar
path_input_local = sys.argv[2] # /Q3final/input.txt
hdfs_input_path = sys.argv[3]+'/input.txt' # /input/input.txt

path_output = sys.argv[4] # /tmp/output311
mapperPath = sys.argv[5]+'/mapper.py' # /Q3final/mapper.py
reducerPath = sys.argv[5]+'/reducer.py' # /Q3final/reducer.py

sum_range = 100000
# iter_count = sys.argv[7] # 10000

file = open(path_input_local, "r")
line = [int(ele) for ele in file.readline().strip().split()]
iter_count = line[0]



os.system('hdfs dfs -rm -r '+hdfs_input_path)
os.system('hdfs dfs -rm -r '+path_output)
os.system('hdfs dfs -mkdir '+hdfs_input_path)
os.system('hdfs dfs -put '+path_input_local+' '+hdfs_input_path)


command = 'hadoop jar '+hadoopPath+' '+'-file '+mapperPath+' '+'-mapper "python3 mapper.py'+'" '+'-file '+reducerPath+' '+'-reducer "python3 reducer.py" '+'-input '+hdfs_input_path+' '+'-output '+path_output

# print(cmd)
os.system(command)
print(str(sum_range) + ' ' + str(iter_count))


# hdfs dfs -put Q2final/input.txt /input
# hadoop jar /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar -file /Q2final/mapper.py -mapper "python3 mapper.py" -file /Q2final/reducer.py -reducer "python3 reducer.py" -input /input/input.txt -output /tmp/output311


# ./runner.py /input/input.txt /Q3final/input.txt /tmp/output311 /Q3final/mapper.py /Q3final/reducer.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar

# hdfs dfs -cat /tmp/output311/part-00000


# ./runner_script.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar /Q3final /input /tmp/output311 /Q3final


# ./runner_script.py /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar /Q3final1/input.txt /input /tmp/output311 /Q3final1