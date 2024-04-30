import random
import csv
import os
import matplotlib.pyplot as plt

def explore_all(dirpath: str, accelerations: list[tuple[float, float, float]]):
    for item in os.listdir(dirpath):
        path = os.path.join(dirpath, item)
        
        if os.path.isfile(path):
            with open(path, "r") as csvfile:
                csv_reader = csv.reader(csvfile)
                next(csv_reader)
                lines = list(csv_reader)
                for line in lines:
                    accelerations.append((float(line[10]), float(line[11]), float(line[12])))
        elif os.path.isdir(path):
            explore_all(path, accelerations)

def create_graph(filename: str, title: str):
    legend = []
    with open(filename, "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)
        
        for pattern_id, line in enumerate(lines):
            accelerations = [float(data) for data in line if data != ""]
            legend.append(f"pattern {pattern_id + 1}")

            plt.plot(accelerations[1:100], label=f"pattern {pattern_id + 1}")

    plt.xlabel('Vacc')
    plt.ylabel('Valeur')
    plt.title(title)
    plt.legend(legend)
    plt.savefig(f"{title}.png")
    plt.close()

create_graph("pattern_x.csv", "pattern x")
create_graph("pattern_y.csv", "pattern y")
create_graph("pattern_z.csv", "pattern z")