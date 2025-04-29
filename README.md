# FILE2GPIO

This application is an attempt to recreate a programming assessment task involving reading GPIO values from a file and writing them to the first 8 GPIO pins of a microprocessor.




https://github.com/user-attachments/assets/c5a6472f-64b4-4a05-ad65-e1077dfc99e4




### Programming Assessment Task
You work for a video game company. You would like to record video game console controller commands to a file and play them back for tool-assisted speedrun tests. To that end, implement the `gpioWriteFile()`, `gpioWriteBuffer()` and `gpioInitializeWrite()` functions
to read from a file with 8 bit GPIO values and write them to the first 8 GPIO pins of a microprocessor, using the usleep() function call to hold the GPIO values before writing the next set of values to the pins.


### Recreation of Programming Assessment Task
This recreation involves a C++ application running on a [BeagleBone Black]([url](https://www.beagleboard.org/boards/beaglebone-black)) board, that reads from the file with 8 bit GPIO values and writes them to 8 GPIO pins (pins specified in a config json file). LEDs are connected to these GPIO pins to indicate the GPIO value on the pin at any given point of time.

The BeagleBone Black used for development is still running Debian Stretch and therefore has the older `sysfs` GPIO interface.

### Assumptions and Modifications made

- The original assessment task only expected implementation of three functions to read values from a file and write them to GPIO pins to be implemented. But since this involves an application for a working project, the object oriented approach of writing separate classes to read from a file (FileReader), and to write the values to 8 GPIO pins (GPIOWriter) lends itself to a cleaner architecture making it more flexible and modular.  

- Since we're emulating video game controller commands, the GPIO value files are generated along with the hold durations for each 8-bit GPIO values like this:

  [8 bit GPIO value1] [32 bit Hold Duration Value1]  
  [8 bit GPIO value2] [32 bit Hold Duration Value2]
  ...

- While the hold duration values were specified in the assessment task in microseconds, values in seconds are used for this project, using `sleep()`  **to make the GPIO writes are more apparent**. They can just as easily be implemented at microsecond precision using `usleep()` or even at nanosecond precision using `nanosleep()`
 

### BeagleBone Black
The BeagleBone Black has two 46 pin headers P8 and P9. Between these two headers, it has a total of 65 GPIO pins although many of the are multiplexed with other functions and need to be explicitly set to GPIO. The board used for this project runs
Debian Stretch and has the `sysfs` interface to GPIO.  

The pins have an absolute GPIO number (GPIOxx) and also a header-based number (P8xx or P9xx).  

**The pins used for this project are: GPIO_66, GPIO_67, GPIO_68, GPIO_69, GPIO_47, GPIO_46, GPIO_65, GPIO_27 (marked in red in the image below):**


![image](https://github.com/user-attachments/assets/75cb21f7-ecd7-46cb-b90f-9695121467db)




The process to use a pin for GPIO from the command line is as follows **(this is handled by the application. The steps outlined below are just for reference)**:
- Set the pinmux to GPIO: `config-pin P8_07 gpio # header based pin number`
- Export the pin: `echo 66 > /sys/class/gpio/export # absolute GPIO pin number`
- Set the mode/direction: `echo out > /sys/class/gpio/gpio66/direction #output`
- Write to value file (1 or 0) for output: `echo 1 > /sys/class/gpio/gpio66/value` or read from value file for input pin: `cat /sys/class/gpio/gpio66/value`
<br>
<br>


    
### Simplified Block Diagram of the Software Application Architecture

 ![image](https://github.com/user-attachments/assets/76134db4-9377-4dcd-8506-a27d15488711)  




### Instructions

#### Compile Application

- Clone the repo, cd into the `docker` directory in the repo and run:  
    `docker build -t bbb-cross-compile .`
- Once the docker image is built, start a container and mount the path to the repo root in it:  
  `docker run -it -v /path/to/repo/root/dir:/home/$USER/file2gpio --workdir /home/$USER/file2gpio bbb-cross-compile:latest`
- Once in the container, in the mounted repo directory, run:
  ```
  mkdir build && cd build
  cmake ../ && make -j
  ```
  
#### Run Application

- Generate the file with GPIO values and hold durations:
   `./scripts/generate_gpio_frames.py <Number of frames> </path/to/file/>`
  For example: `./scripts/generate_gpio_frames.py 100 gpio_cmds`

- To view generated frames:
  `./scripts/print_frames.py /path/to/generated/file`

- Update the `config.json` file with the path to the file with GPIO values:
   `"GPIOCmdsFile": "./gpio_cmds",`

- Run the application with the path to `config.json` file as a command line argument
  `./File2GPIO config.json` 



