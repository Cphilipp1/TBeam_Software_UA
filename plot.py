import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from TDoA import calculate_distance, calculate_distance_xy
from TDoA import Node

def plot(receivers, estimated_transmitter_coords, transmitter):
    fig = plt.figure(figsize=(10, 10))
    ax = fig.add_subplot(111, projection='3d')

    # Plot the receivers
    for i, receiver in enumerate(receivers):
        ax.scatter(receiver.x, receiver.y, receiver.z, label=f"Receiver {i+1}")

    error_distance = str((calculate_distance(Node(estimated_transmitter_coords[0], estimated_transmitter_coords[1], estimated_transmitter_coords[2]), transmitter)))
    error_distance_2d = str((calculate_distance_xy(Node(estimated_transmitter_coords[0], estimated_transmitter_coords[1], estimated_transmitter_coords[2]), transmitter)))

    # Plot the estimated transmitter location
    ax.scatter(*estimated_transmitter_coords, color='red', label='Estimated Transmitter with 3d error distance ' + error_distance[:5] + ' meters')
    ax.scatter(*estimated_transmitter_coords, color='red', label='Estimated Transmitter with 2d error distance ' + error_distance_2d[:5] + ' meters')

    # Plot the true transmitter location
    ax.scatter(transmitter.x, transmitter.y, transmitter.z, color='green', label='True Transmitter')

    # Add legend and labels
    ax.legend()
    ax.set_xlabel('X Coordinate (meters)')
    ax.set_ylabel('Y Coordinate (meters)')
    ax.set_zlabel('Z Coordinate (meters)')
    ax.set_title('Estimated and True Transmitter Locations in 3D')
    plt.show()

def plotMany(x_vals, y_vals, error_grid):
    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(x_vals, y_vals, error_grid, cmap='viridis')

    ax.set_xlabel('X Coordinate')
    ax.set_ylabel('Y Coordinate')
    ax.set_zlabel('Localization Error (meters)')
    ax.set_title('3D Plot of Localization Errors')

    plt.show()
