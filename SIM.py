import numpy as np

# Constants and Parameters
nano_second = 1e-9
speed_of_light = 3e8  # m/s
time_for_fifteen_kilometers = 15000 / speed_of_light  # in seconds
num_receivers = 4

# Function to calculate distance between two nodes
def calculate_distance(node1, node2):
    return np.sqrt((node1.x - node2.x)**2 + (node1.y - node2.y)**2 + (node1.z - node2.z)**2)

def run_SIM(transmitter, receivers, num_nanos):
    # Initialize time of arrivals (ToA)
    toa = np.zeros(num_receivers)

    # Main Simulation Loop
    for t in [i * (nano_second*num_nanos) for i in range(int(time_for_fifteen_kilometers // nano_second) + 1)]:
        for i in range(num_receivers):
            distance_to_receiver = calculate_distance(transmitter, receivers[i])
            time_to_receiver = distance_to_receiver / speed_of_light
            
            # Check if the signal has reached the receiver at this time step
            if t >= time_to_receiver:
                # Record the ToA if not already recorded
                if toa[i] == 0:
                    toa[i] = t
    
    # Calculate Time Differences of Arrival (TDoA) with respect to the first receiver
    return toa - toa[0]
