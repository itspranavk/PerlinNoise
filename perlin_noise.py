from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import sys
import math

def generate_points():
    points = []
    with open('raw.dat', 'r') as f:
        for x, y_points in enumerate(f):
            points.append([])
            for z in y_points.split():
                points[x].append(float(z))
    return points

def square(l):
    for i in range(len(l)):
        l[i] = [x ** 2 for x in l[i]]
    return l

def normalize(l, f):
    return l * f

def render(scale):
    fig = plt.figure()
    ax = plt.axes(projection='3d')

    points = square(generate_points())

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
    
    z = normalize(z, scale/float(max(map(max, z))))

    ax.plot_surface(x, y, z)

    ax.set_xlabel('X Label')
    ax.set_ylabel('Y Label')
    ax.set_zlabel('Z Label')

    plt.show()

if len(sys.argv) >= 5 and len(sys.argv) <= 7:
    num_x = sys.argv[1]
    num_y = sys.argv[2]
    num_octaves = sys.argv[3]
    persistence = sys.argv[4]
    seed_index = 0
    scale = 1

    if len(sys.argv) >= 6:
        seed_index = sys.argv[5]
        if len(sys.argv) == 7:
            scale = int(sys.argv[6])
        elif len(sys.argv) > 7:
            print("Too many arguments!\nUsage: python perlin_noise.py <X> <Y> <Octaves> <Persistence> [Seed Index] [Scale]\n")
    
    subprocess.call(['g++', '-O2', 'noise.cpp', '-o', 'noise'])
    subprocess.call(['./noise', num_x, num_x, num_octaves, persistence, seed_index])

    if scale - 1:
        print("Scaled up by factor of %d" % scale)

    render(scale)

    subprocess.call(['rm', 'noise'])
    subprocess.call(['rm', 'raw.dat'])
else:
    print("Incorrect input format!\nUsage: python perlin_noise.py <X> <Y> <Octaves> <Persistence> [Seed Index] [Scale]\n")