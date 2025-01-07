# Embedded System and Microcomputer Principle (CS301)

It gives hands-on STM32 tutorial and embedded system principles, which is related to Computer Organization but less detailed in architecture and more focused on embedded system design, like bus, Interrupt, I2C, peripherals, etc.

## About

- **Instructor:** `Yuhui Bai (白雨卉)`
- **TA:** `Qing WANG (王晴)`
- **Semester:** `2024 Fall`
- **Textbook:** `NO Textbook, only references`

## Content

- [Assignment](./Assignment): 1 report + 3 STM32 programming
- [Lab](./Lab): 13 lab slides and codes
- [Project](https://github.com/SUSTech-ISACG-Association/ShinxSU7): STM32-based robot car and MiniSTM32 board controller
- [Text](./Text): ARM Cortex-M3, STM32F103RCT6 programming manual and datasheet.

| Lecture | Topic                           | Content                                                      |
| ------- | ------------------------------- | ------------------------------------------------------------ |
| #1      | Introduction to Embedded System | CPU, MCU and SoC<br>Application                              |
| #2      | STM32 MCU & GPIO                | ARM Overview<br>Memory Layout (MMIO)                         |
| #3      | C for Embedded System           | C Recap                                                      |
| #4      | ARM Assembly                    | Arithmetic (and Flags)<br>Load/Store Instructions            |
| #5      | Interrupt                       | Subroutine & Calling Convention<br>Interrupt and NVIC, AIRCR IPRx register |
| #6      | UART                            | UART Protocal<br>USART in STM32                              |
| #7      | Timer                           | Clock Tree<br>STM32 Timers                                   |
| #8      | Advanced Timer                  | Output Compare PWM<br>Input Capture<br>Systick               |
| #9      | I2C & SPI                       | Inter-Integrated Circuit<br>Serial Peripheral Interface Protocol |
| #10     | SD Card & File System           | Storage Devices Overview & SD Card<br>FAT FS                 |
| #11     | Bus                             | Bus Architecture<br>Arbitration                              |
| #12     | ADC                             | Analog-to-Digital Converter (on STM32)                       |
| #13      | DMA & Pipeline                  | Direct Memory Access<br>Bit-banding<br>ARMv7 Pipeline        |
| #14     | Arithmetic                      | Barrel Shifter<br>Ripple Carry, Look Ahead, Carry Select<br>Booth's Algorithm |

## Lab

Each lab has practices to program the peripherals on  STM32 based on the topic and you'll be assigned a MiniSTM32 board through out the semester. There're probably 3 programming assignments in C using STM32CubeMX IDE, and largely they are

- 1st assignment is a simple tool with LCD screen display, LEDs and buttons (KEY0, KEY1, KEY_WKUP), those with GPIOs.
- 2nd assignment deals with UART, Timer or IWDG, those with interrupts.
- 3rd assignment plays with touch screen, and most likely it requires implementing a game.

## Project

The instructors introduced a robot car. Who knows how long the cars will serve. But the principles of project are

- It must contains a MiniSTM board (used in labs and assignments).
- It must includes as much topic as possible, and additional wireless communication (2.4G is based on SPI and bluetooth is based on UART)
