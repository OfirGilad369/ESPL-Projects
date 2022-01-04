import csv
import tkinter as tk
from task1b import moviesInCountryAfterYear

def handle_task1b(event):
	file_path = 'IMDB.csv'
	print("Task1b was executed!")
	moviesInCountryAfterYear(file_path, country.get(), int(year.get()))


if __name__ == '__main__':
	gui = tk.Tk()
	gui.title("Task1b")
	gui.geometry("210x125")
	message1 = tk.Label(text="country name")
	country = tk.Entry()
	message2 = tk.Label(text="year")
	year = tk.Entry()
	task1b_button = tk.Button(text="Task1b")

	message1.pack()
	country.pack()
	message2.pack()
	year.pack()
	task1b_button.pack()

	task1b_button.bind("<Button-1>", handle_task1b)
	gui.mainloop()
