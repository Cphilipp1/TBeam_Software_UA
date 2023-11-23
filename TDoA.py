from scipy.optimize import minimize
import numpy as np
from SIM import run_SIM

# Constants and Parameters
nano_second = 1e-9
speed_of_light = 3e8  # m/s
time_for_fifteen_kilometers = 15000 / speed_of_light  # in seconds
num_receivers = 4

# Define Node class to hold coordinates
class Node:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

# Function to calculate distance between two nodes
def calculate_distance(node1, node2):
    return np.sqrt((node1.x - node2.x)**2 + (node1.y - node2.y)**2 + (node1.z - node2.z)**2)

# Function to calculate distance between two nodes
def calculate_distance_xy(node1, node2):
    return np.sqrt((node1.x - node2.x)**2 + (node1.y - node2.y)**2)

def objective_function_3d(coords, receivers, tdoa, transmitter=Node(0,0,0)):
    x, y, z = coords
    error = 0
    for i in range(1, num_receivers):
        xi, yi, zi = receivers[i].x, receivers[i].y, receivers[i].z
        x0, y0, z0 = receivers[0].x, receivers[0].y, receivers[0].z
        error += (speed_of_light * (tdoa[i]) - (calculate_distance(Node(x, y, transmitter.z), receivers[i]) - calculate_distance(Node(x, y, transmitter.z), receivers[0]))) ** 2
    return error

def calculate_tdoa(transmitter, receivers, num_nanos, z=0):
    sim = True
    if sim: 
        tdoa = run_SIM(transmitter, receivers, num_nanos)
    initial_guess_3d = [0, 0, z]
    result_3d = minimize(lambda coords: objective_function_3d(coords, receivers, tdoa, transmitter=transmitter), initial_guess_3d, method='L-BFGS-B')
    return result_3d.x
