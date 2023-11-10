from TDoA import Node
import numpy as np 
from plot import plot, plotMany
from TDoA import calculate_tdoa, calculate_distance
import time

def main():
    transmitter = Node(500, 400, 0)  # Transmitter
    receivers = [
        Node(0, 0, 0),  # Receiver 1
        Node(0, 1000, 0),  # Receiver 2
        Node(1000, 0, 0),  # Receiver 3
        Node(1000, 1000, 0)  # Receiver 4
    ]
    num_nanos = 100

    estimated_transmitter_coords = calculate_tdoa(transmitter, receivers, num_nanos)
    
    print("Estimated Transmitter Location (x, y, z):", estimated_transmitter_coords)

    single = True

    if single: 
        plot(receivers, estimated_transmitter_coords, transmitter)
    else:
        step = 25 # meters
        # Vectorize the grid creation
        x_range = np.arange(0, 1000, step)
        y_range = np.arange(0, 1000, step)
        x_grid, y_grid = np.meshgrid(x_range, y_range, indexing='ij')

        # TODO: Pre-compute constants if possible

        # Process each grid point
        error_grid = np.zeros_like(x_grid, dtype=float)
        for i in range(x_grid.shape[0]):
            for j in range(y_grid.shape[1]):
                transmitter = Node(x_grid[i, j], y_grid[i, j], 0)  # Transmitter
                estimated_coords = calculate_tdoa(transmitter, receivers, num_nanos)
                error_distance = calculate_distance(Node(*estimated_coords), transmitter)
                error_grid[i, j] = error_distance
        x_vals, y_vals = np.meshgrid(np.arange(0, 1000, step), np.arange(0, 1000, step), indexing='ij')
        plotMany(x_vals, y_vals, error_grid)

if __name__ == "__main__":
    main()
