import csv

def print_to_file_genre_stats_by_country(input_file, output_file, country_name):
    with open(output_file, 'w') as output:
        genre_stats_dictionary = genre_stats_by_country(input_file, country_name)

        for genre_key in genre_stats_dictionary.keys():
            output.write('{}|{}\n'.format(genre_key, genre_stats_dictionary.get(genre_key)))

def genre_stats_by_country(input_file, country_name):
    genres_occurances_dictionary = {}

    with open(input_file, "r") as input:
        csv_reader = csv.DictReader(input, delimiter=',')
        
        for lines in csv_reader:
            genres = lines['genre'].replace(" ", "").split(",")
            countries = lines['country'].replace(" ", "").split(",")

            if country_name in countries:
                for i in range(len(genres)):
                    if genres[i] in genres_occurances_dictionary.keys():
                        current_count = genres_occurances_dictionary[genres[i]]
                        genres_occurances_dictionary[genres[i]] = current_count + 1
                    else:
                        genres_occurances_dictionary[genres[i]] = 1
    
    return genres_occurances_dictionary


if __name__ == '__main__':
    country_name = 'USA'
    input_file = 'IMDB.csv'
    output_file = country_name + '.stats'
    print_to_file_genre_stats_by_country(input_file, output_file, country_name)
