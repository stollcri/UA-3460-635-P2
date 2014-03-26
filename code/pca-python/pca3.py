#!/usr/bin/env python
# -*- coding: utf-8 -*-

import csv
import numpy as np
import matplotlib.pyplot as plt

def read_csv(filename):
	race_dict = {'0': 0, 'A': 1, 'B': 2, 'I': 3, 'K': 4, 'L': 5, 'M': 6, 'N': 7, 'P': 8, 'U': 0, 'W': 9}
	sex_dict = {'0': 0, 'F': 1, 'M': 2, 'U': 0}

	csv_reader=csv.reader(open(filename,"rb"), delimiter=',')
	csv_list=list(csv_reader)
	for i, line in enumerate(csv_list):
		#print csv_list[i][6:17:10]
		csv_list[i][6] = race_dict[line[6]]
		csv_list[i][7] = sex_dict[line[7]]
	return csv_list

N = 10
xTrue = np.linspace(0, 1000, N)
yTrue = 3 * xTrue

# xData = xTrue + np.random.normal(0, 100, N)
# yData = yTrue + np.random.normal(0, 100, N)
# xData = np.reshape(xData, (N, 1))
# yData = np.reshape(yData, (N, 1))
# data = np.hstack((xData, yData))

column_number = {'ssn': 0, 'dln': 1, 'sid': 2, 'fbi': 3, 'dobYear': 4, 'height': 5, 'race': 6, 'gender': 7, 'medicalCount': 8, 'lastMedical': 9, 'lastMedicalDesc': 10, 'offenseCount': 11, 'lastOffense': 12, 'lastOffenseDesc': 13, 'warrantCount': 14, 'lastWarrant': 15, 'lastWarrantBail': 16, 'involveTotal': 17, 'involveCountVictim': 18, 'involveCountSuspect': 19, 'involveCountArrestee': 20, 'involveCountComplainant': 21}
csv_list = read_csv("./out/200204-201102_part.csv")
data = np.asarray(csv_list).astype('int')[:, column_number["height"]:column_number["involveTotal"]:10]
xData = data[:, 0]
yData = data[:, 1]

mu = data.mean(axis=0)
data = data - mu
# data = (data - mu)/data.std(axis=0)  # Uncomment this reproduces mlab.PCA results
eigenvectors, eigenvalues, V = np.linalg.svd(
    data.T, full_matrices=False)
projected_data = np.dot(data, eigenvectors)
sigma = projected_data.std(axis=0).mean()
print(eigenvectors)
def annotate(ax, name, start, end):
    arrow = ax.annotate(name,
                        xy=end, xycoords='data',
                        xytext=start, textcoords='data',
                        arrowprops=dict(facecolor='red', width=2.0))
    return arrow

fig, ax = plt.subplots()
ax.scatter(xData, yData)
#ax.set_aspect('equal')
for axis in eigenvectors:
    annotate(ax, '', mu, mu + sigma * axis)
plt.show()
