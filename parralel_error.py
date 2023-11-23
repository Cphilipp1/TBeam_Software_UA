import unittest
from unittest.mock import patch, MagicMock
from main import parallel_error_calculation

class TestParallelErrorCalculation(unittest.TestCase):
    @patch('main.Pool')
    @patch('main.Array')
    def test_parallel_error_calculation(self, mock_array, mock_pool):
        # Mock the shared memory array
        mock_array.return_value = MagicMock()

        # Mock the Pool
        mock_pool.return_value.__enter__.return_value.map.return_value = [[(0, 0, 10)]]

        # Test data
        x_grid = ...
        y_grid = ...
        receivers = ...
        num_nanos = 31.25
        expected_output = ...

        # Call the function
        result = parallel_error_calculation(x_grid, y_grid, receivers, num_nanos)

        # Assert the result
        self.assertEqual(result, expected_output)

if __name__ == '__main__':
    unittest.main()
