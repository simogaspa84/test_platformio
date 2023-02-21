[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8b764a656de149e88e1192296510dee3)](https://www.codacy.com?utm_source=bitbucket.org&amp;utm_medium=referral&amp;utm_content=systel-sw/iot_vitella&amp;utm_campaign=Badge_Grade)

# README #

This README would normally document whatever steps are necessary to get your application up and running.

### What is this repository for? ###

* The repository is used as VCS for the ESP32 IOT Project

### How do I get set up? ###

* The setup is composed by the ESP32 chip on its board plus another board acting as a USB to serial converter (LC tech company)
* Ide for the development is Visual Studio Code.
* The project was developed adding the extension PlatformIO from Visual Studio code.
* On the file platformio.ini you can find all the information related to the project and used external libraries
* For installing new libraries click on the PlatformIO icon then look for the Quick search menu and look for PIO Home (libraries). Search and install the library you need. 
* For Building ,Uploading and Monitor click on the PlatformIO icon and under the menu esp32dev search for General.
* For Loading remember to keep pressed the reset buttons on the converter and on the esp32 module
* When monitoring remember to change the serial baud rate to 115200 
* The MCU is Espressif ESP32WROOMSE
* Toolchain is xtensa32 2.50200.97
* For the deployment refer to the confluence pages of the project 

### Contribution guidelines ###

* Code review is performed by default review

### Who do I talk to? ###

* Simone Gasparella simone.gasparella@systel.it