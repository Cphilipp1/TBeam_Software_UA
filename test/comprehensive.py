import unittest

if __name__ == '__main__':
    # Directory containing your test files
    test_dir = 'test'
    # Discover and run tests
    suite = unittest.defaultTestLoader.discover(test_dir, pattern='test_*.py')
    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(suite)
