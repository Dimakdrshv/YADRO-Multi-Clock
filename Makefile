MIK32_HAL_DIR=hardware/mik32-hal
MIK32_SHARED_DIR=hardware/mik32v2-shared

BUILD_DIR=build

SERIAL_PORT?=/dev/ttyUSB0
SERIAL_BOUDRATE?=57600

.PHONY: clean flash monitor dump

build_app: update_submodules $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

update_submodules: $(MIK32_HAL_DIR)/README.md $(MIK32_SHARED_DIR)/README.md

$(MIK32_HAL_DIR)/README.md:
	git submodule update --init hardware/mik32-hal

$(MIK32_SHARED_DIR)/README.md:
	git submodule update --init hardware/mik32v2-shared

$(BUILD_DIR):
	cmake -G Ninja -B $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

flash:
	python3 $(MIK32_UPLOADER_DIR)/mik32_upload.py build/app/YADRO_MULTI_CLOCK.hex --run-openocd \
	--openocd-exec /usr/bin/openocd \
	--openocd-target $(MIK32_UPLOADER_DIR)/openocd-scripts/target/mik32.cfg \
	--openocd-interface $(MIK32_UPLOADER_DIR)/openocd-scripts/interface/ftdi/mikron-link.cfg \
	--adapter-speed 500 --mcu-type MIK32V2

monitor:
	picocom $(SERIAL_PORT) -b $(SERIAL_BOUDRATE) --omap crcrlf --echo

dump:
	$(MIK32_TOOLCHAIN_DIR)/riscv-none-elf-objdump -d $(BUILD_DIR)/app/YADRO_MULTI_CLOCK.elf > dump