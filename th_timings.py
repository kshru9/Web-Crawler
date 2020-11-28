import csv
import matplotlib.pyplot as plt
import sys

d_Time = []
p_Time = []
u_Time = []

with open('OUTPUT/th_timings.csv') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	for row in csv_reader:
		if len(row) == 0:
			break
		else:
			d_Time.append(float(row[0]))
			p_Time.append(float(row[1]))
			u_Time.append(float(row[2]))

			

x = [i for i in range(len(d_Time))]

plt.plot(x, d_Time, linewidth = 2)
plt.plot(x, p_Time, linewidth = 2)
plt.plot(x, u_Time, linewidth = 2)
plt.legend(['download time', 'parse time', 'var updation time'])
plt.xlabel('Threads')
plt.xlabel('Elapsed time in microseconds')

plt.savefig('OUTPUT/graph_'+sys.argv[1]+'.png', dpi=300)