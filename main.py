from TDoA import Node
import numpy as np 
from plot import plot, plotMany
from TDoA import calculate_tdoa, calculate_distance
import time
from multiprocessing import Pool, Array
import ctypes

def worker_function(batch_args):
    """ Process a batch of grid points """
    results = []
    for args in batch_args:
        i, j, x_val, y_val, receivers, num_nanos = args
        transmitter = Node(x_val, y_val, 0)
        estimated_coords = calculate_tdoa(transmitter, receivers, num_nanos)
        error_distance = calculate_distance(Node(*estimated_coords), transmitter)
        results.append((i, j, error_distance))
    return results

def parallel_error_calculation(x_grid, y_grid, receivers, num_nanos):
    # Create shared memory array
    shared_array = Array(ctypes.c_double, x_grid.size)
    error_grid = np.frombuffer(shared_array.get_obj())  # No copy
    error_grid = error_grid.reshape(x_grid.shape)

    # Prepare arguments for multiprocessing
    grid_points = [(i, j, x_grid[i, j], y_grid[i, j], receivers, num_nanos) for i in range(x_grid.shape[0]) for j in range(y_grid.shape[1])]
    batch_size = 10  # adjust based on your system's capabilities
    batches = [grid_points[i:i + batch_size] for i in range(0, len(grid_points), batch_size)]

    # Use multiprocessing Pool
    with Pool() as pool:
        batch_results = pool.map(worker_function, batches)

    # Populate the error_grid with results from batches
    for batch in batch_results:
        for i, j, error in batch:
            error_grid[i, j] = error

    return error_grid

def main():

    transmitter = Node(500, 400, 0)  # Transmitter
    num_nanos = 100
    receivers = [
        Node(0, 0, 0),  # Receiver 1
        Node(0, 1000, 0),  # Receiver 2
        Node(1000, 0, 0),  # Receiver 3
        Node(1000, 1000, 0)  # Receiver 4
    ]

    single = False
    if single:         
        estimated_transmitter_coords = calculate_tdoa(transmitter, receivers, num_nanos)
        print("Estimated Transmitter Location (x, y, z):", estimated_transmitter_coords)
        plot(receivers, estimated_transmitter_coords, transmitter)

    else:
        step = 25 # meters
        # Vectorize the grid creation
        x_range = np.arange(0, 1000, step)
        y_range = np.arange(0, 1000, step)
        x_grid, y_grid = np.meshgrid(x_range, y_range, indexing='ij')

        # TODO: Pre-compute constants if possible
        # Perform parallel computation
        start_time = time.perf_counter()
        error_grid = parallel_error_calculation(x_grid, y_grid, receivers, num_nanos)
        x_vals, y_vals = np.meshgrid(np.arange(0, 1000, step), np.arange(0, 1000, step), indexing='ij')
        end_time = time.perf_counter()

        elapsed_time = end_time - start_time
        print(str(start_time) + " :: " + str(end_time))
        print(f"Function executed in {elapsed_time} seconds.")

        plotMany(x_vals, y_vals, error_grid, np.mean(error_grid))

if __name__ == "__main__":
    main()
