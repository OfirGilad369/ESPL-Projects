import csv
import tkinter as tk
from task1a import movieCountByCountry

def handle_task1a(event):
	file_path = 'IMDB.csv'
	print("Task1a was executed!")
	movieCountByCountry(file_path, output_path.get())


if __name__ == '__main__':
	gui = tk.Tk()
	gui.title("Task1a")
	gui.geometry("210x80")
	message = tk.Label(text="save file name")
	output_path = tk.Entry()
	task1a_button = tk.Button(text="Task1a")

	message.pack()
	output_path.pack()
	task1a_button.pack()

	task1a_button.bind("<Button-1>", handle_task1a)
	gui.mainloop()
