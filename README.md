Personal project to show basic understanding of FreeRTOS to monitor temperature data to send from a computer to a Pi Pico 2 and back over USB/UART
Sensor used: Combo Temp+Humidity Sensor KY015 (DHT11)

Credit: 
https://github.com/sbehnke/freertos-pico-template - used as reference to setup the project, utilized updated submodule instead, highly recommend looking at this to understand how to get a general understanding on how to setup freertos on any pi pico, may add a separte section on how it generally works at a basic understanding
https://github.com/vmilea/pico_dht - used this library to make the sensor work

Things to do: try and use two cores, add a spi/i2c screen for another task
