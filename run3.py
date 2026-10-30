from run import RadarProcessRunner
import argparse
import signal

# Parsing the 
parser = argparse.ArgumentParser()
parser.add_argument("yaml_file", nargs='?',
    help='First file path') 
parser.add_argument("second_file", nargs='?',
    help="Second file")
parser.add_argument("third_file", nargs='?',
    help="Third file")
args = parser.parse_args() 
yaml_filename = args.yaml_file
second_filename = args.second_file
third_filename = args.third_file

# Checking that all 3 files are given
if(yaml_filename == None):
    print("Missing the first config file")
    exit(1)
if(second_filename == None):
    print("Missing the second config file")
    exit(1)
if(third_filename == None):
    print("Missing the third config file")
    exit(1)

# Printing to help ensure the right files are being used
print("first file is", yaml_filename)
print("second file is", second_filename)
print("third file is", third_filename)

runner = RadarProcessRunner(yaml_filename)
runner2 = RadarProcessRunner(second_filename)
runner3 = RadarProcessRunner(third_filename)

def sigint_handler(signum, frame):
    runner.stop() # On Ctrl-C, attempt to stop radar process

# Run through SDR process using the first config file
print("START SETUP FOR FIRST")
runner.setup()
print("First setup complete")
runner.run()
signal.signal(signal.SIGINT, sigint_handler)
runner.wait()
runner.stop()

# Run through SDR process using the second config file
print("START SETUP FOR SECOND")
runner2.setup()
runner2.run()
signal.signal(signal.SIGINT, sigint_handler)
runner2.wait()
runner2.stop()

# Run through SDR process using the third config file
print("START SETUP FOR THIRD")
runner3.setup()
runner3.run()
signal.signal(signal.SIGINT, sigint_handler)
runner3.wait()
runner3.stop()