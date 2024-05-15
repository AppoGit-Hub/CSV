import random
import csv
import os
import matplotlib.pyplot as plt

plt.rcParams['axes.facecolor'] = 'none'

mode = "white"

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

    plt.figure(facecolor='none')

    with open(filename, "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)

        for pattern_id, line in enumerate(lines):
            accelerations = [float(data) for data in line if data != ""]
            legend.append(f"mouvement {int(line[0])}")

            plt.plot(accelerations[1:], label=f"mouvement {int(line[0])}")

    plt.xlabel('Vacc', color=mode)
    plt.ylabel('Valeur', color=mode)
    plt.title(title, color=mode)
    plt.legend(legend, labelcolor=mode)

    plt.xticks(color=mode)
    plt.yticks(color=mode)

    plt.gca().spines['bottom'].set_color(mode)
    plt.gca().spines['top'].set_color(mode)
    plt.gca().spines['right'].set_color(mode)
    plt.gca().spines['left'].set_color(mode)

    plt.savefig(f"{title}.png")
    plt.close()

def view_testset(filename: str, view_index: int, title: str):
    with open(filename, "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        lines = list(csv_reader)

        line = lines[view_index - 1]
        for data in line:
            accelerations = [float(data) for data in line if data != ""]
            #plt.hist(accelerations[1:])
            
            plt.plot(accelerations[1:])
        
    plt.savefig(f"{title}.png")
    plt.close()


view_testset("testset.csv", 1, "testset")


"""
create_graph("pattern.csv", "pattern")
create_graph("pattern_attitude_pitch.csv", "pattern pitch")
create_graph("pattern_attitude_roll.csv", "pattern roll")
create_graph("pattern_attitude_yaw.csv", "pattern yaw")
create_graph("pattern_gravity_x.csv", "pattern gravity x")
create_graph("pattern_gravity_y.csv", "pattern gravity y")
create_graph("pattern_gravity_z.csv", "pattern gravity z")
create_graph("pattern_rotation_rate_x.csv", "pattern rotation rate x")
create_graph("pattern_rotation_rate_y.csv", "pattern rotation rate y")
create_graph("pattern_rotation_rate_z.csv", "pattern rotation rate z")
create_graph("pattern_user_acceleration_x.csv", "pattern user acceleration x")
create_graph("pattern_user_acceleration_y.csv", "pattern user acceleration y")
create_graph("pattern_user_acceleration_z.csv", "pattern user acceleration z")
"""