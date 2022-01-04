import csv

def moviesInCountryAfterYear(file_path, country, year):
	count = 0

	with open(file_path) as csv_file:
		imdb = csv.DictReader(csv_file)
		
		for i, row in enumerate(imdb):
			if i != 0: 	
				countries = (row["country"]).split(', ')
				movieYear = int(row["year"])
				if movieYear > year and country in countries:
					count += 1

	print(f"Number of movies after {year} in {country}: {count}.")


if __name__ == '__main__':
	country_name = 'USA'
	year = 1980
	file_path = 'IMDB.csv'
	moviesInCountryAfterYear(file_path, country_name, year)
    