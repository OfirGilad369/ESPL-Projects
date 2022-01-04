import csv
import matplotlib.pyplot as plt 

def plotMoviesYearsHistogram(file_path):
	with open(file_path) as csv_file:
		years = []
		imdb = csv.DictReader(csv_file)
		for i, row in enumerate(imdb):
			if i != 0: 	
				years.append(int(row["year"]))
	
	plt.hist(years)

	plt.show()


if __name__ == '__main__':
	file_path = 'IMDB.csv'
	plotMoviesYearsHistogram(file_path)
    