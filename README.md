# TM4C1294-Keypad-and-BCD-Interface
GPIO-based interface for a 4x4 hexadecimal keypad and 7-segment BCD display using the TI TM4C1294 microcontroller. Developed in C with Code Composer Studio.
This repository contains the C-based firmware for interfacing a 4x4 matrix keypad and a 7-segment BCD display using the TI Tiva C Series TM4C1294NCPDT microcontroller. The project was implemented at a register level to demonstrate a deep understanding of GPIO hardware configuration and signal timing.

🛠️ Hardware Overview
The project utilizes a custom lab setup featuring:

Microcontroller: TI Tiva C Series TM4C1294 (ARM Cortex-M4F).

Input Device: 4x4 Hexadecimal Keypad (Matrix scanned via Port M).

Output Device: 7-Segment BCD (Binary Coded Decimal) Display (Driven via Port K).

Environment: Code Composer Studio (CCS).

🚀 Key Engineering Challenges Addressed
1. Matrix Scanning Algorithm
Implemented a 4-step scanning routine. By cycling columns (X1-X4) to LOW and reading the rows (Y1-Y4) with internal pull-up resistors, the code accurately identifies the intersection of a pressed key.

2. Capacitive Load & Runtime Delays
As noted in the hardware specifications, external circuitry introduces capacitive loads that cause voltage transitions to follow an exponential curve.

Solution: I implemented a calibrated wait() function between column switching and row reading to ensure the signal level is stable before the MCU samples the data.

3. Edge Detection & Multi-Press Error Handling
To make the interface "user-friendly" and robust, the firmware includes:

Single-Press Logic: Uses a last_key state variable to ensure a character is only registered once per physical press.

Conflict Detection: An error-checking routine that detects if more than one key is pressed simultaneously, outputting an 'X' error state and a serial warning message.

📁 Code Structure
configure_keypad(): Register-level setup for Port M (Clock gating, Digital Enable, Direction, and Pull-up resistors).

read_keypad(): The core logic for scanning the 4x4 matrix.

display_key(): Converts ASCII characters to 4-bit BCD values and updates Port K.

📸 Demonstration
The setup successfully captures the hex input and translates it to the BCD display in real-time. For example, pressing the '6' key triggers the scanning logic to output 0x06 to the 7-segment driver.
<img width="1861" height="2048" alt="WhatsApp Image 2026-05-07 at 14 29 14" src="https://github.com/user-attachments/assets/02307055-0681-4e16-b2d1-3536135d74bf" />





https://github.com/user-attachments/assets/9ba84b27-1bd6-47d4-9601-5a46b39b6951


