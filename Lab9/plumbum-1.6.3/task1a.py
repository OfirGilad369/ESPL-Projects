import csv

def movieCountByCountry(file_path, output_path):
	countries = []
	countriesCountDict = {}

	with open(file_path) as csv_file:
		imdb = csv.DictReader(csv_file)
		for i, row in enumerate(imdb):
			if i != 0: 	
				countries += (row["country"]).split(', ')
	
	for country in countries:
		if country in countriesCountDict.keys():
			countriesCountDict[country] += 1
		else:
			countriesCountDict[country] = 1
	
	with open(output_path, 'w') as output:
		for k, v in sorted(countriesCountDict.items(), key=lambda x: x[1], reverse=True):
			output.write('{}|{}\n'.format(k, v))


if __name__ == '__main__':
	file_path = 'IMDB.csv'
	output_path = 'movies.stats'
	movieCountByCountry(file_path, output_path)
