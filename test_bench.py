import csv
import matplotlib.pyplot as plt

time_elap = []
x = []
with open('OUTPUT/crawler_timings.csv') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	for row in csv_reader:
		if len(row) == 0:
			break
		else:
			x.append(int(row[0]))
			time_elap.append(int(row[1]))



plt.plot(x, time_elap, linewidth = 4)
plt.xlabel("number of threads")
plt.ylabel("time elapsed to crawl")

plt.savefig('OUTPUT/crawler_analytics.png', dpi=300)