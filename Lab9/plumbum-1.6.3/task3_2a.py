import csv
import tkinter as tk
from task2a import print_to_file_genre_stats

def handle_task2a(event):
	file_path = 'IMDB.csv'
	print("Task2a was executed!")
	print_to_file_genre_stats(file_path, output_path.get())


if __name__ == '__main__':
	gui = tk.Tk()
	gui.title("Task2a")
	gui.geometry("210x80")
	message = tk.Label(text="save file name")
	output_path = tk.Entry()
	task2a_button = tk.Button(text="Task2a")

	message.pack()
	output_path.pack()
	task2a_button.pack()

	task2a_button.bind("<Button-1>", handle_task2a)
	gui.mainloop()
