# UART Communication with EDAC and CRC

Implementation of UART Communication between 2 STM32 boards with added *Error Detection and Correction* (EDAC) and *Cyclic Redundancy Check* (CRC).  


## Overview
This project aims to simulate the communication process between a satellite and a ground station using 2 STM32 boards connected via UART.
One of the boards would act as the transmitter (encoder), while the other would act as the receiver (decoder).

## Setup and Configuration
### 1. Clone this repository
```bash
git clone https://github.com/HexterLyman/UART-Communicator.git
```
Note: There are 2 types of files - the .ino files and the .c files. The .ino files are part of the Arduino IDE sketches. 
**Avoid removing them from the directory it is in!**

### 2. Arduino IDE Setup and Configuration
#### a. Setting up the Boards Manager
1. Open Arduino IDE.
2. Go to **Preferences/Settings**.
3. Locate the field titled **Additional Boards Manager URLs**.
4. Paste the following URL into the field:
```
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
```
4. Click **OK**.


#### b. Install STM32 Core
1. Go to **Tools > Board > Boards Manager**.
2. In the search bar, type **STM32**.
3. Locate the entry **STM32 MCU based boards (by STMicroelectronics)**.
4. Click **Install**.

#### c. Select Your Board
1. After installation, go to **Tools > Board > STM32 MCU based boards**.
2. In the side menu, find and select your board type of choice under **Board Type**.
3. Set the Board Part Number under **Tools > Board Part Number** based on the board of your choice.

#### d. Select Port and Upload Method
1. Go to **Tools > Port** and select the corresponding port.
2. Under **Tools > Upload Method**, choose **STM32CubeProgrammer (SWD)**.

### 3. Hardware Configuration
- Board 1 TX (PD5, CN9-6) → Board 2 RX (PD6, CN9-4)
- Board 1 RX (PD6, CN9-4) → Board 2 TX (PD5, CN9-6) (for potential bidirectional communication)
- Common ground connection between both boards (GND)

### 4. Flash the Firmware

Note: It is recommended to use 2 different devices with one acting as the transmitter / encoder, and the other acting as the receiver / decoder.

## Acknowledgements
Here are the people who helped contribute to this project (in no particular order):
- Ali Alsaad
- Merve Aygün
- Zehranur Patan
- Royatul Rauf
- Lesina Saidaeva

 
