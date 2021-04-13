import matplotlib
import matplotlib.pyplot as plt
import numpy as np


f = open("points.txt", "r")
lines = f.readlines()
x_coordinates = []
y_coordinates = []
for i in lines:
    i = i.split(",")
    x_coordinates.append(float(i[0]))
    y_coordinates.append(float(i[1]))
#print(x_coordinates, y_coordinates)
plt.scatter(x_coordinates, y_coordinates)

f.close()

g = open("ab.txt", "r")
lines = g.readlines()
error = lines[0]
C = lines[1]
lines = lines[2:]
plt.title("error value : "+error+", C val : "+C)
for i in lines:
    x1, x2, a, b = map(float, i.split(","))
    y1 = x1*a+b
    y2 = x2*a+b
    plt.plot([x1, x2], [y1, y2])
plt.show()
g.close()
