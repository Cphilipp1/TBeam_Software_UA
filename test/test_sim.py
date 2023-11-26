import unittest
import numpy as np
from src.SIM import run_SIM, calculate_distance
from src.TDoA import Node

class TestTDoAModule(unittest.TestCase):

    def setUp(self):
        # Set up some nodes for testing
        self.node1 = Node(0, 0, 0)
        self.node2 = Node(3, 4, 0)

    def test_calculate_distance(self):
        # Test the calculate_distance function
        calculated_distance = calculate_distance(self.node1, self.node2)
        expected_distance = 5  # Based on the 3-4-5 right triangle
        self.assertAlmostEqual(calculated_distance, expected_distance, places=5)

    def test_run_SIM(self):
        # Test the run_SIM function with a basic setup
        transmitter = Node(0, 0, 0)
        receivers = [Node(0, 300000000, 0), Node(300000000, 0, 0),
                     Node(300000000, 300000000, 0), Node(0, 0, 300000000)]
        num_nanos = 1
        toa = run_SIM(transmitter, receivers, num_nanos, noise_mean=0, noise_std=0, seed=42)

        # The exact values to expect can depend on the specific implementation
        # Here, we might expect all ToAs to be very similar since the setup is symmetric
        self.assertEqual(len(toa), 4)
        for t in toa[1:]:
            self.assertAlmostEqual(toa[0], t, places=5)

if __name__ == '__main__':
    unittest.main()
