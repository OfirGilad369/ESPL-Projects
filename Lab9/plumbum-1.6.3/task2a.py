import csv

def print_to_file_genre_stats(input_file, output_file):
    with open(output_file, 'w') as output:
        genre_stats_dictionary = genre_stats(input_file)

        for genre_stat in genre_stats_dictionary:
            genre = list(genre_stat.keys())[0]
            output.write('{}|{}\n'.format(genre, genre_stat.get(genre)))

def genre_stats(input_file):
    genres_duration_dictionary = {}

    with open(input_file, "r") as input:
        csv_reader = csv.DictReader(input, delimiter=',')
        
        for lines in csv_reader:
            genres = lines['genre'].replace(" ", "").split(",")

            for i in range(len(genres)):
                if genres[i] in genres_duration_dictionary.keys():
                    current_count, current_duration = genres_duration_dictionary[genres[i]]
                    genres_duration_dictionary[genres[i]] = (current_count + 1, current_duration + int(lines['duration']))
                else:
                    genres_duration_dictionary[genres[i]] = (1, int(lines['duration']))
    
    return genres_averages(genres_duration_dictionary)
    
def genres_averages(genres_duration_dictionary):
    genres_averages_dictionary = []

    for genre, (count, total_duration) in genres_duration_dictionary.items():
        genres_averages_dictionary.append({genre : (total_duration / count)})

    return genres_averages_dictionary


if __name__ == '__main__':
    input_file = 'IMDB.csv'
    output_file = 'genre.stats'
    print_to_file_genre_stats(input_file, output_file)
