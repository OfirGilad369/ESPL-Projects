import csv
import tkinter as tk
from task2b import print_to_file_genre_stats_by_country

def handle_task2b(event):
	file_path = 'IMDB.csv'
	print_to_file_genre_stats_by_country(file_path, output_path.get(), country.get())
	print("Task2b was executed!")


if __name__ == '__main__':
	gui = tk.Tk()
	gui.title("Task2b")
	gui.geometry("210x125")
	message1 = tk.Label(text="country name")
	country = tk.Entry()
	message2 = tk.Label(text="save file name")
	output_path = tk.Entry()
	task2b_button = tk.Button(text="Task2b")

	message1.pack()
	country.pack()
	message2.pack()
	output_path.pack()
	task2b_button.pack()

	task2b_button.bind("<Button-1>", handle_task2b)
	gui.mainloop()
