import matplotlib.pyplot as plt
import numpy as np

def read_data(file):
    data = []
    with open(file, 'r') as f:
        lines = f.readlines()

    return data

def plot_data(data):
    plt.plot(data)
    plt.show()