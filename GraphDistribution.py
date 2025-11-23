# A program designed to plot the distribution of a
# set of numbers, line by line in a text file.
# Demetri Sotirelis 13 March 2025

import matplotlib.pyplot as plt
import random
def main():

	f = open("Output.txt")
	numbersList = []
	for line in f:
		line = line.strip()
		numbersList.append(float(line))
	f.close()
	
	
	# Plotting a basic histogram
	#plt.hist(numbersList, bins = ( int(((max(numbersList)//1)+1)) - int((min(numbersList)//1)) + 1)  )
	plt.hist(numbersList, bins = 100)
	# Adding labels and title
	plt.xlabel('Number')
	plt.ylabel('Frequency of Number')
	plt.title('Characterizaion of Mersenne Twister')
	

	# Display the plot
	print("Minimum Value: {} Maximum Value: {}".format(min(numbersList),max(numbersList)))
	plt.show()
	print("Plots shown")
	
main()
		