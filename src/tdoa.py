import numpy as np

def calculate_distance(point1, point2):
    return np.linalg.norm(np.array(point1) - np.array(point2))

def calculate_tdoa(distance1, distance2, speed_of_light=3e8):
    return (distance2 - distance1) / speed_of_light
