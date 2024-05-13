import matplotlib.pyplot as plt

def read_data(filename):
    data = {}
    with open(filename, 'r') as file:
        lines = file.readlines()
        for line in lines:
            values = line.split(';')
            data[values[0]] = [float(x) for x in values[1:]] 
    return data

def plot_data(data, title, keyvalues):
    for keyvalue in keyvalues:
        time_values = data[keyvalue + "_Time"]
        data_values = data[keyvalue]
        plt.plot(time_values, data_values)
    
    plt.xlabel('Time (ms)')
    plt.ylabel('Value')
    plt.title(title)
    plt.legend(keyvalues)
    plt.grid(True)

    plt.show()


if __name__ == "__main__":
    data = read_data("Data1.txt")

    for key in data.keys():
        if not str(key).endswith("_Time"):
            plot_data(data, "Attribute evolution", [str(key)])