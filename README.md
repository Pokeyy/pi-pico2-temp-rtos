# Pi Pico 2 (RP2350) FreeRTOS Temperature Sensor CLI

Simple project that aims to use FreeRTOS to showcase implementation of two tasks:
- Task to monitor temperature and humidity from the KY-015 Sensor and display the data over the VSCode Pi Pico SDK Serial Monitor via USB
- Another task to use USB to send data from a computer by inputting keywords to the Pi Pico 2 to convert the data displayed between Celsius and Fahrenheit

## Features
- Uses Pi Pico VSCode Extension
- FreeRTOS
- Uses both cores for each task
- Transmitting and Receiving Data through Serial Monitor over USB
- CMake

## Serial Commands
- When running the project, the Serial Monitor will start displaying in Celsius, to get Fahrenheit type:
  
  ```fahrenheit;```
- To change back to Celsius:

  ```celsius;```

## Hardware
- Pi Pico 2 (RP2350)
- KY-015 (DHT11 + Humidity Combo Sensor)

### Misc:
- If cloning this project or the template referenced, please be sure to clone recursively or update the submodules
  
  ```git submodule update --init```
- If you would like to learn how to utilize multiple cores on your MCU with FreeRTOS, search up things on ```vTaskCoreAffinitySet```


### Plan to add:
- ST7735 LCD Display to handle temperature + humidity display (make a task with this and use queues)
- Buttons to interact with display or something

### Credit: 
https://github.com/sbehnke/freertos-pico-template - used as reference to setup the project, repointed the freertos used for the pi pico, highly recommend looking at this to understand how to get a general understanding on how to setup freertos on any pi pico, may add a separte section on how to generally set it up

https://github.com/FreeRTOS/FreeRTOS-Kernel - the kernel used in this project and the one referenced

https://github.com/vmilea/pico_dht - library to make the sensor KY-015 work
