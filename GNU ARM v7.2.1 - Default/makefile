################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32XG13X/GCC/subdir.mk
-include protocol/bluetooth/bt_mesh/src/subdir.mk
-include platform/middleware/glib/glib/subdir.mk
-include platform/middleware/glib/dmd/display/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/emdrv/gpiointerrupt/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: course-project-victorkronberg_update.axf

# Tool invocations
course-project-victorkronberg_update.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\efr32bg13p632f512gm48.ld" -Wl,--undefined,bg_application_properties,--undefined,__Vectors,--undefined,__aeabi_uldivmod,--undefined,ceil,--undefined,__nvm3Base -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="course-project-victorkronberg_update.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o course-project-victorkronberg_update.axf -Wl,--start-group "./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./buttons.o" "./dcd.o" "./gatt_db.o" "./gecko_main.o" "./graphics.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/display.o" "./hardware/kit/common/drivers/displayls013b7dh03.o" "./hardware/kit/common/drivers/displaypalemlib.o" "./hardware/kit/common/drivers/i2cspm.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/drivers/retargetio.o" "./hardware/kit/common/drivers/retargetserial.o" "./hardware/kit/common/drivers/si7013.o" "./hardware/kit/common/drivers/tempsens.o" "./hardware/kit/common/drivers/udelay.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emlib/src/em_adc.o" "./platform/emlib/src/em_assert.o" "./platform/emlib/src/em_burtc.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_eusart.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_letimer.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_se.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./platform/middleware/glib/dmd/display/dmd_display.o" "./platform/middleware/glib/glib/bmp.o" "./platform/middleware/glib/glib/glib.o" "./platform/middleware/glib/glib/glib_bitmap.o" "./platform/middleware/glib/glib/glib_circle.o" "./platform/middleware/glib/glib/glib_font_narrow_6x8.o" "./platform/middleware/glib/glib/glib_font_normal_8x8.o" "./platform/middleware/glib/glib/glib_font_number_16x20.o" "./platform/middleware/glib/glib/glib_line.o" "./platform/middleware/glib/glib/glib_polygon.o" "./platform/middleware/glib/glib/glib_rectangle.o" "./platform/middleware/glib/glib/glib_string.o" "./protocol/bluetooth/bt_mesh/src/bg_application_properties.o" "./protocol/bluetooth/bt_mesh/src/mesh_lib.o" "./protocol/bluetooth/bt_mesh/src/mesh_sensor.o" "./protocol/bluetooth/bt_mesh/src/mesh_serdeser.o" "./src/Si7021.o" "./src/adc.o" "./src/client.o" "./src/gecko_ble_errors.o" "./src/gecko_mesh.o" "./src/gpio.o" "./src/i2c.o" "./src/lcd_display.o" "./src/light_sensor.o" "./src/log.o" "./src/main.o" "./src/scheduler.o" "./src/sensor.o" "./src/sensor_client.o" "./src/server.o" "./src/soil_sensor.o" "./src/timer.o" "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\protocol\bluetooth\lib\EFR32XG13X\GCC\libbluetooth_mesh.a" "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\protocol\bluetooth\lib\EFR32XG13X\GCC\binapploader.o" "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg13_gcc_release.a" "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\protocol\bluetooth\lib\EFR32XG13X\GCC\libpsstore.a" "C:\Users\vkronber\SimplicityStudio\v4_workspace\course-project-victorkronberg_update\protocol\bluetooth\lib\EFR32XG13X\GCC\libcoex.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: course-project-victorkronberg_update.hex'
	arm-none-eabi-objcopy -O ihex "course-project-victorkronberg_update.axf" "course-project-victorkronberg_update.hex"
	@echo ' '

	@echo 'Building bin file: course-project-victorkronberg_update.bin'
	arm-none-eabi-objcopy -O binary "course-project-victorkronberg_update.axf" "course-project-victorkronberg_update.bin"
	@echo ' '

	@echo 'Building s37 file: course-project-victorkronberg_update.s37'
	arm-none-eabi-objcopy -O srec "course-project-victorkronberg_update.axf" "course-project-victorkronberg_update.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "course-project-victorkronberg_update.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) course-project-victorkronberg_update.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
