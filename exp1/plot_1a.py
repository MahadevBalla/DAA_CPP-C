import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('output.csv')

n_values = data['n']
columns = data.columns[1:]

def plot_each_function(data, n_values, function_names):
    for function in function_names:
        plt.figure(figsize=(8, 5))
        plt.plot(n_values, data[function], label=function, color='blue')
        plt.xlabel('n')
        plt.ylabel('Function Value')
        plt.title(f'Plot of {function}')
        plt.legend()
        plt.grid(True)
        plt.show()

plot_each_function(data, n_values, columns)