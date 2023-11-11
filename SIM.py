import numpy as np

# Constants and Parameters
nano_second = 1e-9
speed_of_light = 3e8  # m/s
time_for_fifteen_kilometers = 15000 / speed_of_light  # in seconds
num_receivers = 4

# Function to calculate distance between two nodes
def calculate_distance(node1, node2):
    return np.sqrt((node1.x - node2.x)**2 + (node1.y - node2.y)**2 + (node1.z - node2.z)**2)

def run_SIM(transmitter, receivers, num_nanos, noise_mean=0, noise_std=1e-8, seed=None):
    """
    Simulates the propagation of light with Gaussian noise.

    :param transmitter: The transmitter node.
    :param receivers: List of receiver nodes.
    :param num_nanos: Number of nanoseconds to simulate.
    :param noise_mean: Mean of the Gaussian noise (default is 0).
    :param noise_std: Standard deviation of the Gaussian noise (default is 1 ns).
    :param seed: Seed for the random number generator for reproducibility (default is None).
    :return: Time Differences of Arrival (TDoA) with Gaussian noise.
    """
    # Set the seed for reproducibility if provided
    if seed is not None:
        np.random.seed(seed)

    toa = np.zeros(num_receivers)

    for t in [i * (nano_second * num_nanos) for i in range(int(time_for_fifteen_kilometers // nano_second) + 1)]:
        for i in range(num_receivers):
            distance_to_receiver = calculate_distance(transmitter, receivers[i])
            time_to_receiver = distance_to_receiver / speed_of_light
            
            if t >= time_to_receiver:
                if toa[i] == 0:
                    # Add Gaussian noise to the ToA
                    noise = np.random.normal(noise_mean, noise_std)
                    toa[i] = t + noise

    return toa - toa[0]
