.PHONY: software-test
.PHONY: hardware-test
software-test: build-cxx software-cxx-test python-test
hardware-test: build-cxx hardware-cxx-test

.PHONY: build-cxx
build-cxx:
	@echo "Building C++ project..."
	cmake -S sdr -B sdr/build
	cmake --build sdr/build

.PHONY: software-cxx-test
software-cxx-test:
	@echo "Running C++ software tests..."
	ctest --test-dir sdr/build --output-on-failure -E "gpsLock|check10MhzLock|checkAndSetTime|detectChannels|setRFParams|refLoLockDetect|setupGpio"

.PHONY: python-test
python-test:
	@echo "Running Python tests..."
	conda run -n uhd pytest tests/


.PHONY: hardware-cxx-test
hardware-cxx-test:
	@echo "Running C++ hardware tests.."
	ctest --test-dir sdr/build --output-on-failure