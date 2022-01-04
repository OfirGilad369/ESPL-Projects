import csv
import tkinter as tk
from task1c import plotMoviesYearsHistogram

def handle_task1c(event):
	file_path = 'IMDB.csv'
	print("Task1c was executed!")
	plotMoviesYearsHistogram(file_path)


if __name__ == '__main__':
	gui = tk.Tk()
	gui.title("Task1c")
	gui.geometry("210x55")
	message = tk.Label(text="click the button")
	task1c_button = tk.Button(text="Task1c")

	message.pack()
	task1c_button.pack()

	task1c_button.bind("<Button-1>", handle_task1c)
	gui.mainloop()
