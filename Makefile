OUTDIR ?= ettus

.PHONY: software-test
.PHONY: hardware-test
software-uhd-test: build-uhd-cxx software-uhd-cxx-test python-test
software-pynq-test: build-pynq-cxx software-pynq-cxx-test python-test
hardware-uhd-test: build-uhd-cxx hardware-uhd-cxx-test
hardware-pynq-test: build-pynq-cxx hardware-pynq-cxx-test

.PHONY: build-cxx
build-uhd-cxx:
	@echo "Building C++ project..."
	cmake -S sdr -B sdr/ettus/build -DRADAR_MODE="uhd"
	cmake --build sdr/ettus/build

build-pynq-cxx:
	@echo "Building C++ project..."
	cmake -S sdr -B sdr/amd/build -DRADAR_MODE="pynq"
	cmake --build sdr/amd/build


.PHONY: software-cxx-test
software-uhd-cxx-test:
	@echo "Running C++ software tests..."
	ctest --test-dir sdr/ettus/build --output-on-failure -E "gpsLock|check10MhzLock|checkAndSetTime|detectChannels|setRFParams|refLoLockDetect|setupGpio"

.PHONY: software-cxx-test
software-pynq-cxx-test:
	@echo "Running C++ software tests..."
	ctest --test-dir sdr/amd/build --output-on-failure -E "gpsLock|check10MhzLock|checkAndSetTime|detectChannels|setRFParams|refLoLockDetect|setupGpio"


.PHONY: python-test
python-test:
	@echo "Running Python tests..."
	conda run -n uhd pytest tests/


.PHONY: hardware-uhd-cxx-test
hardware-uhd-cxx-test:
	@echo "Running C++ Usrp hardware tests.."
	ctest --test-dir sdr/ettus/build --output-on-failure


.PHONY: hardware-pynq-cxx-test
hardware-pynq-cxx-test:
	@echo "Running C++ Pynq hardware tests.."
	ctest --test-dir sdr/amd/build --output-on-failure