import unittest
from unittest.mock import patch
from src.SIM import Node
from src.main import worker_function, parallel_error_calculation
import numpy as np

class TestTDoAModule(unittest.TestCase):

    def setUp(self):
        # Define some common setup for the tests
        self.transmitter = Node(500, 400, 2)
        self.receivers = [Node(0, 0, 5), Node(0, 1000, 2), Node(1000, 0, 3), Node(1000, 1000, 1)]
        self.num_nanos = 31.25

    def test_worker_function(self):
        # Test the worker_function with a known setup
        batch_args = [(0, 0, 500, 400, self.receivers, self.num_nanos, 2, True)]
        results = worker_function(batch_args)
        expected_error_distance = 4.837487304208234  # Error from tested and seeded TDoA
        for _, _, error_distance in results:
            self.assertAlmostEqual(error_distance, expected_error_distance, places=2)

    def test_parallel_error_calculation(self):
        # Test the parallel_error_calculation function
        step = 25  # meters
        x_range = np.arange(0, 1000, step)
        y_range = np.arange(0, 1000, step)
        x_grid, y_grid = np.meshgrid(x_range, y_range, indexing='ij')

        parallel_error_calculation(x_grid, y_grid, self.receivers, self.num_nanos, self.transmitter)
        return True
    
if __name__ == '__main__':
    unittest.main()
