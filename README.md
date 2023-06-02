# MIPS-Assembler

A program that assembles MIPS instructions into machine code

## The Program

This program is a supposed simulation of the MIPS architecture. It takes a .asm input file which has code written in assembly. The program then translates those instructions into machine code and then attempts to execute said instructions.

## How it Works
### Parsing

The program reads the assembly code line by line. It first parses through the entire file to remove the comments in assembly code denoted by the hashtag (#) character. It outputs the edited code as a stringstream. This stringstream is then ran through two parsing algorithms. The first one runs through the entire stream and searches for the labels. If a label in the .data section is found, it segments the line accordingly and stores the name of the label, its address, its contents and the type of data into a vector. The properties of the label are tied together as a class called Label, which is what is pushed into the vector. If a label in the .text section is found, it segments the line accordingly and stores the name of the label and its address as an instance of the Label class and pushes it into the labels vector. The data type of this label would be defaulted to instruction and would contain an empty string as its content.

### Translation

The next parsing algorithm is the main function that will change the assembly code into machine code. This algorithm jumps straight to the .text section of the assembly code and translates these instructions one by one into machine code. If the algorithm encounters a label, it will proceed to skip it and move on to the next instruction. The output of this algorithm are lines of machine code (0s and 1s) as a stringstream. 

## Installation

To install the MIPS-Assembler, follow these steps:

1. Clone this repository to your local machine:

    `https://github.com/BlazedEngima/MIPS-Assembler.git`

2. Ensure that you have a C/C++ compiler and cmake installed.

## Running

To run the program, follow these steps:

1. Create a build directory in the project file

    `mkdir .\build`

2. Build the program with cmake

    `cd build`
    `cmake ../`
    `cd ..`
    `cmake -B .\build --target all -j 8`

3. Run the program

    `cd build`
    `.\Simulator [Input_file] [Output_file]`

