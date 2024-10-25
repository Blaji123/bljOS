# bljOS

## Description

**bljOS** is a custom operating system kernel designed from the ground up. Built to run with the GRUB bootloader, bljOS is my own OS implementation that includes low-level hardware communication, essential drivers, multitasking, and the beginnings of a filesystem.

## Table of contents

- [Features](#features)
- [Tech Stack](#tech-stack)
- [Installation](#installation)

## Features
 
### Bootloader
 
- Utilizes GRUB for initialization and booting.

### Kernel
 
- **Port Communication** : Direct interaction with hardware devices via I/O ports.
- **GDT (Global Descriptor Table)** : Set up memory segmentation and security.
- **IDT (Interrupt Descriptor Table)** : Handles interrupt and exceptions.

### Drivers

- **Mouse and Keyboard** : Basic input device support for user interraction
- **VGA** : Basic video graphics adapter for text and graphical display
- **RTC** : Keeps track of system time and date.
- **AHCI (Advanced Host Controller Interface)** : Support for SATA drives.
- **AMD am79c973** : Network chip implementation (the default network chip for virtualbox)

### GUI

- **Minimalist GUI** : Simple GUI structure for basic interactivity.

### Network

- **Network stack** : Basic network stack implementation enabling data transmission and reception (TCP, UDP).

### Filesystem

- **Filesystem (WIP)** : Initial structure for file storage and management.

### Multitasking

- **Multitasking support** : Allows for concurrent process handling.

## Tech Stack

**C**, **GAS Assembly**, **Makefile**

## Installation

### For using

- Install Oracle VirtualBox on your computer, download the iso file from here https://github.com/Blaji123/bljOS/releases/tag/v1.0, and set up a virtual machine using the iso file

### For running

- To run the code you'll need a linux distribution system,
- After that if you don't already have them you should open a terminal and run
        sudo apt-get install
            g++
            binutils
            libc6-dev-i386
- Then you should open a terminal in the os folder and write make run, make sure you have Oracle VirtualBox installed and have a set up of a virtual machine using the iso file





