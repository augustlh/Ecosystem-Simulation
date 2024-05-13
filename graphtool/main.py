# Import the necessary libraries
import matplotlib.pyplot as plt
from typing import List, Dict
import os

def read_data(filename : str) -> Dict[str, List[float]]:
    """
    Reads the data from a file and returns a structured dictionary with the data, where the keys are the target names and the values are the data values.
    
    Parameters:
    `filename (str)`: The name of the file to read the data from.

    Data should be in the following format (as defined per the Statistics library in C++):

    `
    Target;x1;x2;...;xn;
    Target_Time;t1;t2;...;tn;
    `
    where `Target` is the name of the target, `x1`, `x2`, ..., `xn` are the values of the target and `t1`, `t2`, ..., `tn` are the time values. `Target` is then set as the key of the dictionary and the values are a list of the data values.
    """
    filepath = os.path.join("..", "Build", filename)
    data: Dict[str, list[float]] = {}

    with open(filepath, 'r') as file:
        lines = file.readlines()
        for line in lines:
            values = line.split(';')
            data[values[0]] = [float(x) for x in values[1:]] 
    return data

def plot_data(data : Dict[str, List[float]], title : str, keyvalues : List[str]) -> None:
    """
    Plots the data from the dictionary `data` for the keys in `keyvalues` with the title `title`. The data is plotted using the matplotlib library.

    Parameters:
    `data (dict)`: The dictionary with the data.
    `title (str)`: The title of the plot.
    `keyvalues (list)`: The keys of the data to plot.

    """
    for keyvalue in keyvalues:
        time_values = [t / 1000 for t in data[keyvalue + "_Time"]]
        data_values = data[keyvalue]
        plt.plot(time_values, data_values)

        plt.xlabel('Time (s)')
        plt.ylabel('Value')
        plt.title(title)
        plt.legend([keyvalue])
        plt.grid(True)

        if not os.path.exists("plots"):
            os.makedirs("plots")
        path = os.path.join("plots", f"{keyvalue}.png")
        plt.savefig(path, dpi=600)
        plt.close()


if __name__ == "__main__":
    data = read_data("Data1.txt")
    plot_data(data, "Attribute evolution", [str(key) for key in data.keys() if not str(key).endswith("_Time")])
    print("Plots generated. Please view the plots in the 'plots' directory.")