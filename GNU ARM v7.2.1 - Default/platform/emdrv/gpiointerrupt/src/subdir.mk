################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/emdrv/gpiointerrupt/src/gpiointerrupt.c 

OBJS += \
./platform/emdrv/gpiointerrupt/src/gpiointerrupt.o 

C_DEPS += \
./platform/emdrv/gpiointerrupt/src/gpiointerrupt.d 


# Each subdirectory must supply rules for building sources it contributes
platform/emdrv/gpiointerrupt/src/gpiointerrupt.o: ../platform/emdrv/gpiointerrupt/src/gpiointerrupt.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DMESH_LIB_NATIVE=1' '-DINCLUDE_LOGGING=1' '-D__HEAP_SIZE=0x1200' '-D__STACK_SIZE=0x1000' '-DHAL_CONFIG=1' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\CMSIS\Include" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\hardware\kit\common\drivers" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\middleware\glib\glib" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\radio\rail_lib\protocol\ble" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emlib\src" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\protocol\bluetooth\bt_mesh\src" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emlib\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\middleware\glib\dmd\ssd2119" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\hardware\kit\common\bsp" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\middleware\glib" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\hardware\kit\common\halconfig" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\middleware\glib\dmd" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\common\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\uartdrv\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\bootloader\api" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\sleep\src" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\radio\rail_lib\common" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\sleep\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\halconfig\inc\hal-config" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\middleware\glib\dmd\display" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\gpiointerrupt\src" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\vkronber\SimplicityStudio\v4_workspace\soc-btmesh-sensor-server_2\platform\radio\rail_lib\protocol\ieee802154" -Os -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/emdrv/gpiointerrupt/src/gpiointerrupt.d" -MT"platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


