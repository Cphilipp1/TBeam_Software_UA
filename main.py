from TDoA import Node
from plot import plot
from TDoA import calculate_tdoa

def main():
    transmitter = Node(500, 400, 30)  # Transmitter
    receivers = [
        Node(1000, 0, 50),  # Receiver A
        Node(1000, 900, 10),  # Receiver B
        Node(0, 800, 50),  # Receiver C
        Node(0, 0, 10)   # Receiver D
    ]
    num_nanos = 100

    estimated_transmitter_coords = calculate_tdoa(transmitter, receivers, num_nanos)
    
    print("Estimated Transmitter Location (x, y, z):", estimated_transmitter_coords)

    plot(receivers, estimated_transmitter_coords, transmitter)

if __name__ == "__main__":
    main()
