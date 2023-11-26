
TDoA_Software_UA Repository Overview
This repository contains the implementation of a Time Difference of Arrival (TDoA) system in Python. The system is designed to simulate and calculate the position of a transmitter based on the time difference of signals received at multiple receivers.
File Descriptions
1. SIM.py
Purpose: This file contains functions related to the simulation of signal propagation and noise addition.
Functions:
calculate_distance(node1, node2): Calculates the Euclidean distance between two nodes.
run_SIM(transmitter, receivers, num_nanos, noise_mean, noise_std, seed): Simulates the propagation of signals from a transmitter to receivers, adding Gaussian noise to model real-world conditions.
2. TDoA.py
Purpose: Core file for the TDoA calculations and position estimation.
Components:
Node class: Represents a node (transmitter or receiver) with x, y, and z coordinates.
calculate_distance(node1, node2): Same as in SIM.py, for distance calculation.
calculate_distance_xy(node1, node2): Calculates the 2D distance (ignoring z-coordinate) between two nodes.
objective_function_3d(coords, receivers, tdoa, transmitter): Objective function used in optimization for 3D position estimation.
calculate_tdoa(transmitter, receivers, num_nanos, z): Calculates the estimated position of the transmitter using TDoA data.
3. main.py
Purpose: Main script to run the TDoA calculations and visualize results.
Functions:
worker_function(batch_args): Processes a batch of grid points for parallel computation.
parallel_error_calculation(x_grid, y_grid, receivers, num_nanos, transmitter): Calculates the error in estimated positions over a grid.
main(): Main function to execute the TDoA calculations and plotting.
4. parralel_error.py
Purpose: Contains unit tests for the parallel_error_calculation function.
Tests:
Unit tests to validate the correctness and efficiency of the parallel error calculation.
5. plot.py
Purpose: Provides functions for plotting the results of TDoA calculations.
Functions:
plot(receivers, estimated_transmitter_coords, transmitter): Plots the estimated and actual positions of the transmitter in 3D.
plotMany(x_vals, y_vals, error_grid, avg_error): Creates a 3D plot of localization error across a grid.
6. README.md
Purpose: Provides an overview and documentation for the repository.
7. dependencies
Purpose: Lists the dependencies required to run the software.

