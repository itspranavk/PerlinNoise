from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt

def generate_points():
    points = []
    with open('raw.dat', 'r') as f:
        for x, y_points in enumerate(f):
            points.append([])
            for z in y_points.split():
                points[x].append(float(z))
    return points

def f(x, y, p):
    z = []
    for i in range(len(x[0])):
        z.append(p[int(round(x[0][i]))][int(round(y[0][i]))])
    z_final = []
    for i in range(len(x)):
        z_final.append(z)
    return np.asarray(z_final)

fig = plt.figure()
ax = plt.axes(projection='3d')

points = generate_points()

X = []
Y = []
for i in range(len(points)):
    X.append([])
    Y.append([])
    for j in range(len(points)):
        X[i].append(i)
        Y[i].append(j)

x = np.reshape(X, (len(points), len(points)))
y = np.reshape(Y, (len(points), len(points)))
z = np.reshape(points, (len(points), len(points)))

ax.plot_surface(x, y, z)

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()