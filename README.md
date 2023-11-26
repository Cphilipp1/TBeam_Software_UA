Prerequisites
Before you begin, ensure you have the following installed on your system:

Python 3.11
pip (Python package manager)
You can download Python from [Google](https://www.python.org), and pip is included with Python.

Installation
1. Step 1: Clone the Repository
First, clone the repository to your local machine. You can do this by running:

bash
git clone [URL to Repository]
cd [Repository Name]
Replace [URL to Repository] with the URL of the Git repository and [Repository Name] with the name of the directory that Git creates.

2. Step 2: Install Dependencies
Install the required Python packages using pip and the dependencies.txt file:

bash
Copy code
'pip install -r dependencies.txt'
This command will automatically download and install all the packages listed in dependencies.txt.

3. Step 3: Navigate to the src Directory
Change your current directory to the src directory:
bash
cd src

Running the Simulation
To run the simulation, execute main.py from within the src directory:

Testing the Simulation
'python3.11 -m test.comprehensive'
This will run all unit test cases on all funcional components of the simulation

bash
'python3.11 main.py'
This command will start the simulation program and generate an interactive 3d simulated environment. Follow any on-screen prompts or instructions to interact with the simulation.

Additional Information
Make sure to follow any specific instructions or requirements detailed in the simulation program or provided by the developers.
If you encounter any issues, refer to the troubleshooting section below or contact the support team.
Troubleshooting
Dependency Installation Issues: If you encounter errors during the installation of dependencies, ensure your Python and pip are up to date.
Import Errors: Make sure you are running main.py from within the src directory, as the relative imports depend on the current directory structure.
Python Version: This program is designed for Python 3.x. Running it on Python 2.x may lead to unexpected issues.

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

