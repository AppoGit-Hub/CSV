import random
import csv
import os
import matplotlib.pyplot as plt

plt.rcParams['axes.facecolor'] = 'none'

mode = "black"

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
            legend.append(f"Mouvement {int(line[0])}")

            plt.plot(accelerations[1:], label=f"Mouvement {int(line[0])}")

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
            plt.plot(accelerations[3:100])
    
    plt.title(int(line[0]))
    plt.savefig(f"{title}.png")
    plt.close()

def view_evaluation(filename: str, title: str):
    colors = ['#FF5733', '#3399FF', '#33FF57', '#FFA500', '#800080', '#FFFF00']

    plt.figure(facecolor='none')

    with open(filename, "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        data = []
        for line in csv_reader:
            data.append([float(data) for data in list(line)[1:] if data != ''])
    
        percentages = [[(val / sum(row)) * 100 for val in row] for row in data]

        x = range(1, len(data) + 1)
        bottom = [0] * len(data)
        for i in range(len(data[0])):
            plt.bar(x, [percentages[j][i] for j in range(len(data))], bottom=bottom, color=colors[i], label=f'Movement {i+1}')
            bottom = [bottom[j] + percentages[j][i] for j in range(len(data))]

    plt.xlabel('Mouvement', color=mode)
    plt.ylabel('Estimations', color=mode)
    plt.title('Evaluation', color=mode)
    plt.legend(labelcolor=mode)
    plt.savefig(f"{title}.png")
    plt.close()

def view_performance(filename: str, title: str):
    plt.figure(facecolor='none')
    
    with open(filename, "r") as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)
        data = []
        for index, line in enumerate(csv_reader, 1):
            rights = float(list(line)[index])
            all = sum([float(data) for data in list(line)[1:] if data != ''])
            print([rights, all, rights / all])
            
            data.append((rights / all) * 100)
    

    plt.bar(range(1, len(data) + 1), data)
    plt.xlabel('Mouvement', color=mode)
    plt.ylabel('Performance', color=mode)
    plt.title(f"Evaluation {title}", color=mode)
    plt.savefig(f"{title}.png")
    plt.close()

create_graph("pattern.csv", "pattern")
view_evaluation("evaluation.csv", "evaluation")
view_performance("evaluation.csv", "performance")


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