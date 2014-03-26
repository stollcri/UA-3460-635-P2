#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import csv
import numpy

from matplotlib.mlab import PCA

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import pylab

def plot_pca_3d(result):
	x = []
	y = []
	z = []
	for item in result.Y:
		x.append(item[0])
		y.append(item[1])
		z.append(item[2])

	plt.close('all') # close all latent plotting windows
	fig1 = plt.figure() # Make a plotting figure
	ax = Axes3D(fig1) # use the plotting figure to create a Axis3D object.
	pltData = [x,y,z]
	ax.scatter(pltData[0], pltData[1], pltData[2], 'bo') # make a scatter plot of blue dots from the data

	# make simple, bare axis lines through space:
	xAxisLine = ((min(pltData[0]), max(pltData[0])), (0, 0), (0,0)) # 2 points make the x-axis line at the data extrema along x-axis 
	ax.plot(xAxisLine[0], xAxisLine[1], xAxisLine[2], 'r') # make a red line for the x-axis.
	yAxisLine = ((0, 0), (min(pltData[1]), max(pltData[1])), (0,0)) # 2 points make the y-axis line at the data extrema along y-axis
	ax.plot(yAxisLine[0], yAxisLine[1], yAxisLine[2], 'r') # make a red line for the y-axis.
	zAxisLine = ((0, 0), (0,0), (min(pltData[2]), max(pltData[2]))) # 2 points make the z-axis line at the data extrema along z-axis
	ax.plot(zAxisLine[0], zAxisLine[1], zAxisLine[2], 'r') # make a red line for the z-axis.

	# label the axes 
	ax.set_xlabel("x-axis label") 
	ax.set_ylabel("y-axis label")
	ax.set_zlabel("z-axis label")
	ax.set_title("Principal Component Analysis plot")
	plt.show() # show the plot


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


def process_pca(filename):
	csv_list = read_csv(filename)
	# csv_array = numpy.array(csv_list).astype('int')
	# results = PCA(csv_array)
	# plot_pca_3d(results)

	# csv_array = numpy.asarray(csv_list).astype('int')
	# numpy.savetxt("pca.csv", csv_array, delimiter=",")

	csv_array = numpy.asarray(csv_list).astype('int')[:, 6:17:10]
	print csv_array.shape
	plot_pca(csv_array)


def PCA(data, dims_rescaled_data=2):
	"""
	returns: data transformed in 2 dims/columns + regenerated original data
	pass in: data as 2D NumPy array
	"""
	from scipy import linalg as LA
	mn = numpy.mean(data, axis=0)
	# mean center the data
	data -= mn
	# calculate the covariance matrix
	C = numpy.cov(data.T)
	# calculate eigenvectors & eigenvalues of the covariance matrix
	evals, evecs = LA.eig(C)
	# sorted them by eigenvalue in decreasing order
	idx = numpy.argsort(evals)[::-1]
	evecs = evecs[:,idx]
	evals = evals[idx]
	# select the first n eigenvectors (n is desired dimension
	# of rescaled data array, or dims_rescaled_data)
	evecs = evecs[:,:dims_rescaled_data]
	# carry out the transformation on the data using eigenvectors
	data_rescaled = numpy.dot(evecs.T, data.T).T
	# reconstruct original data array
	data_original_regen = numpy.dot(evecs, dim1).T + mn
	return data_rescaled, data_original_regen

def plot_pca(data):
	clr1 =  '#2026B2'
	fig = plt.figure()
	ax1 = fig.add_subplot(111)
	data_resc, data_orig = PCA(data)
	ax1.plot(data_resc[:,0], data_resc[:,1], '.', mfc=clr1, mec=clr1)
	# ax1.plot(data[:,0], data[:,1], '.', mfc=clr1, mec=clr1)
	plt.show()


if __name__ == "__main__":
	column_number = {'ssn': 0, 'dln': 1, 'sid': 2, 'fbi': 3, 'dobYear': 4, 'height': 5, 'race': 6, 'gender': 7, 'medicalCount': 8, 'lastMedical': 9, 'lastMedicalDesc': 10, 'offenseCount': 11, 'lastOffense': 12, 'lastOffenseDesc': 13, 'warrantCount': 14, 'lastWarrant': 15, 'lastWarrantBail': 16, 'involveTotal': 17, 'involveCountVictim': 18, 'involveCountSuspect': 19, 'involveCountArrestee': 20, 'involveCountComplainant': 21}

	parser = argparse.ArgumentParser()
	parser.add_argument("infile", help="The source data file", type=argparse.FileType('r'), metavar='file')
	args = parser.parse_args()

	try:
		process_pca(args.infile.name)
	except KeyboardInterrupt:
		exit(0)
