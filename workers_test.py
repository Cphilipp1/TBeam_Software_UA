import unittest
from main import Node
from unittest.mock import patch
from main import worker_function

class TestWorkerFunction(unittest.TestCase):
    @patch('TDoA.calculate_tdoa')
    @patch('TDoA.calculate_distance')
    def test_worker_function(self, mock_calculate_distance, mock_calculate_tdoa):
        # Setup mock return values
        mock_calculate_tdoa.return_value = (100, 200)
        mock_calculate_distance.return_value = 50

        receivers = [
            Node(0, 0, 5),  # Receiver 1
            Node(0, 1000, 2),  # Receiver 2
            Node(1000, 0, 3),  # Receiver 3
            Node(1000, 1000, 1)  # Receiver 4
        ]

        # Sample input
        batch_args = [(0, 0, 10, 20, receivers, 50)]
        expected_output = [(0, 0, 38.22302970723278)]

        # Call the worker function
        result = worker_function(batch_args)
    
        # Assert the result
        self.assertEqual(result, expected_output)

if __name__ == '__main__':
    unittest.main()
