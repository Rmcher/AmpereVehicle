# AmpereVehicle

Ampere Vehicle/TDPS 2022-2023 (Group 2) - Team-26

The **Ampere Vehicle** project is a team project mainly for a rover completed by a group of students who enjoy working with electronic equipment in the **Team Design and Project Skills (TDPS) UESTC3010** at the UESTC, Glasgow College. The rover can navigate bumpy cobblestones, recognize arrows, track lines, drop a ping pong ball in a trash can, and communicate with a computer. The repository mainly contains the following contents: two main control programs of the rover based on the STM32G474RE main control board (for Patio 1 and Patio 2), image recognition code, and line inspection code based on OpenMV. Because of the particularity of our deployment, the codes of devices including ultrasound and gyroscopes are encapsulated in the main control program.

## Prerequisites

You should have a relevant background in **embedded development**. In terms of software capabilities, you need to have a basic understanding of **C language** and **python**. In terms of hardware, due to the limitations of the repository, you need to have the ability to build hardware from scratch according to the instruction file.

You need to use the [Keil Studio Cloud](https://studio.keil.arm.com/auth/login/) online compiler as the interpreter of the main control program.
And You also need download the official [IDE of OpenMV](https://openmv.io/pages/download) as the compiler for the python file of the vision module.

## Implement

Explain how to run the automated tests for this system

### STM32G474RE

You need to put the two files patio1.c and patio2.c into the active project created in keil studio cloud, and import the online header file according to the instructions.

### OpenMV

The two python files can be opened with your familiar IDE, but for a better debugging experience, you can use the OpenMV IDE.

## Authors

* **Ampere Vehicle Team 26** - *Initial work* - Writer [Wu Binghong](https://github.com/Rmcher)
