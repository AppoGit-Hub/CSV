import csv
import os
import matplotlib.pyplot as plt


def display_at(index: int, to: int):
    legend = []

    with open("move_x.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader) 
        line = lines.pop(index)
        accelerations = [float(data) for data in line if data != ""]
        legend.append("x")
        type = accelerations[0]
        plt.plot(accelerations[1:to], label=f"x")

    with open("move_y.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader) 
        line = lines.pop(index)
        accelerations = [float(data) for data in line if data != ""]
        legend.append("y")
        plt.plot(accelerations[1:to], label=f"y")

    with open("move_z.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader) 
        line = lines.pop(index)
        accelerations = [float(data) for data in line if data != ""]
        legend.append("z")
        plt.plot(accelerations[1:to], label=f"z")

    plt.xlabel('Vacc')
    plt.ylabel('Valeur')
    plt.title(f"File: {index}, Type: {type}")
    plt.legend(legend)
    plt.savefig(f"moves.png")
    plt.close()

def display_sum(movement_type: str, max_acc = 100):
    with open("move_x.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)
        accelerations = [] 
        for line in lines:
            correct_line = line[1:max_acc]
            type = line[0]
            if type == movement_type:
                for data in correct_line:
                    if data != "":
                        accelerations.append(float(data))
        sum_acc = [sum(items) for items in zip(*accelerations)]
        plt.plot(sum_acc)
        
    with open("move_y.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)
        accelerations = [] 
        for line in lines:
            correct_line = line[1:max_acc]
            type = line[0]
            if type == movement_type:
                accelerations.append([float(data) for data in correct_line if data != ""])
        sum_acc = [sum(items) for items in zip(*accelerations)]
        plt.plot(sum_acc)

    with open("move_z.csv", "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)
        accelerations = [] 
        for line in lines:
            correct_line = line[1:max_acc]
            type = line[0]
            if type == movement_type:
                accelerations.append([float(data) for data in correct_line if data != ""])
        sum_acc = [sum(items) for items in zip(*accelerations)]
        plt.plot(sum_acc)

    plt.xlabel('Vacc')
    plt.ylabel('Valeur')
    plt.title(f"File")
    plt.savefig(f"sum_move_{movement_type}.png")
    plt.close()

display_sum("1")
display_sum("2")
display_sum("3")
display_sum("4")
display_sum("5")
display_sum("6")