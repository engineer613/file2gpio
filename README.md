# FILE2GPIO

This application is an attempt to recreate a coding challenge involving reading GPIO values from a file and writing them to the first 8 GPIO pins of a microprocessor. A BeagleBone Black running Debian Stretch was used for this recreation.

### Coding Challenge Statement (paraphrased)
You work for a video game company. You would like to record video game console controller commands to a file and play them back for tool-assisted speedrun tests. To that end, implement the gpioWriteFile(), gpioWriteBuffer() and gpioInitializeWrite() functions
to read from a file with 8 bit GPIO values and write them to the first 8 GPIO pins of a microprocessor, using the usleep() function call to hold the GPIO values before writing the next set of values to the pins.


### Recreation of coding challenge
This recreation involves:
1. Generating a file with 8 bit GPIO values and hold durations in seconds first. 
2. Then the application running on a BeagleBone Black reads from the GPIO value file, writes those values to specified GPIO pins (pins specified in a config json file), toggling LEDs connected to those pins for hold durations read from the file.


### Assumptions and Modifications made

While the coding challenge expected functions to be implemented all of which were implied to be running in the same thread, writing separate classes (with their own threads) to read from a file (FileReader), and to write the values read from file
to the GPIO pins (GPIOWriter) is a cleaner architecture making it more flexible and modular.  

- Since we're emulating video game controller commands, the GPIO value files are generated along with the hold durations for each 8-bit GPIO values like this:

  [8 bit GPIO value1] [32 bit Hold Duration Value1]  
  [8 bit GPIO value2] [32 bit Hold Duration Value2]
  ...

- While the hold duration is specified in the challenge as a value in microseconds which can be implemented using `usleep()` on most Linux operating systems, values in seconds are used for this project, using `sleep()`  so the GPIO writes (to pins connected to LEDs) are more apparent.
 

### BeagleBone Black
The BeagleBone Black has two 46 pin headers P8 and P9. It has a total of 65 GPIO pins although many of the are multiplexed with other functions and need to be explicitly set to GPIO through it's `sysfs` interface. The board used for this project runs
Debian Stretch and has the `sysfs` interface to GPIO.


![image](https://github.com/user-attachments/assets/75cb21f7-ecd7-46cb-b90f-9695121467db)





**The pins used for this project are: GPIO_66, GPIO_67, GPIO_68, GPIO_69, GPIO_47, GPIO_46, GPIO_65, GPIO_27 (marked in red in the picture above)**


The pins have an absolute GPIO number (GPIOxx) and also a header-based number (P8xx or P9xx). The process to use a pin for GPIO from the command line is as follows **(this is handled by the application. The commands are just for reference)**:
- Set the pinmux to GPIO: `config-pin P8_07 gpio # header based pin number`
- Export the pin: `echo 66 > /sys/class/gpio/export # absolute GPIO pin number`
- Set the mode/direction: `echo out > /sys/class/gpio/gpio66/direction #output`
- Write to value file (1 or 0) for output: `echo 1 > /sys/class/gpio/gpio66/value` or read from value file for input pin: `cat /sys/class/gpio/gpio66/value`
`


To be able to use a pin for GPIO, the process is as follows


    
### Simplified Schematic of the Software Application Architecture

 ![image](https://github.com/user-attachments/assets/2e70c118-3868-46e6-a9e9-d204e58e19e7)

  
### Instructions

- Generate the file with GPIO values and hold durations:
   `./scripts/generate_gpio_frames.py <Number of frames> </path/to/file/>`
  For example: `./scripts/generate_gpio_frames.py 100 gpio_cmds`

- To view generated frames:
  `./scripts/print_frames.py /path/to/generated/file`

- Update the `config.json` file with the path to the file with GPIO values:
   `"GPIOCmdsFile": "./gpio_cmds",`

- Run the application with the path to `config.json` file as a command line argument
  `./File2GPIO config.json` 



