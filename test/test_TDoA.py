import unittest
from unittest.mock import patch
import numpy as np
from scipy.optimize import minimize
from src.TDoA import calculate_distance, calculate_distance_xy, objective_function_3d, calculate_tdoa
from src.SIM import Node

class TestPositioningModule(unittest.TestCase):

    def setUp(self):
        # Set up some nodes for testing
        self.node1 = Node(0, 0, 0)
        self.node2 = Node(3, 4, 0)
        self.node3 = Node(3, 4, 12)
        self.receivers = [Node(0, 0, 5), Node(0, 1000, 2), Node(1000, 0, 3), Node(1000, 1000, 1)]
        self.transmitter = Node(500, 400, 2)
        self.num_nanos = 31.25

    def test_calculate_distance(self):
        calculated_distance = calculate_distance(self.node1, self.node2)
        expected_distance = 5  # Based on the 3-4-5 right triangle
        self.assertAlmostEqual(calculated_distance, expected_distance, places=5)

    def test_calculate_distance_xy(self):
        calculated_distance = calculate_distance_xy(self.node1, self.node2)
        expected_distance = 5  # Same as above, ignoring the z-coordinate
        self.assertAlmostEqual(calculated_distance, expected_distance, places=5)

    def test_objective_function_3d(self):
        # Assumed known TDoA values for a scenario
        known_tdoa = [0, 0.000001, 0.000002, 0.000003]
        error = objective_function_3d([500, 400, 2], self.receivers, known_tdoa, self.transmitter)
        self.assertAlmostEqual(error, 961909.3701009087, places=5) # empirically derived

    @patch('src.SIM.run_SIM')
    def test_calculate_tdoa(self, mock_run_sim):
        # Mocking run_SIM to return a predetermined TDoA
        mock_run_sim.return_value = np.array([0, 0.000001, 0.000002, 0.000003])
        
        estimated_coords = calculate_tdoa(self.transmitter, self.receivers, self.num_nanos, self.transmitter.z, test=True)
        # Checking if estimated coordinates are close to actual transmitter coordinates
        error_distance = calculate_distance(Node(*estimated_coords), self.transmitter)
        self.assertAlmostEqual(error_distance, 4.837487304208234, places=1) # empirically derived

if __name__ == '__main__':
    unittest.main()
