#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>


#define Null_Address 0x00000000
#define ADDR_INSTR 0x00400000
#define ADDR_DATA 0x00500000
#define MEM_SIZE 0x05B8D80

/* 
 * Defining a new class to tie labels with their addresses, if the label contains content and a data type
 * Then it is a label in the .data field which contains data, if it does not have these then it is
 * a label in the .text section, which stores only the label name and address
 */
class Label{
    public:
        /* Constructor */
        Label(std::string name, int32_t address, std::string data_type = "instruction", std::string content = "") {
            this->name = name;
            this->address = address;
            this->data_type = data_type;
            this->content = content;
        }

        /* Getters */
        std::string getName() {return name;}
        int32_t getAddress() {return address;}
        std::string getData_type() {return data_type;}
        std::string getContent() {return content;}
        void setAddress(int32_t new_address) {this->address = new_address;}

    private:
        /* Instace variables */
        std::string name;
        int32_t address;
        std::string data_type;
        std::string content;

        /* Overloading */
        friend bool operator==(Label label, std::string string);
        friend bool operator==(std::string string, Label label);
        friend std::ostream &operator<<(std::ostream &os, Label label);
};

/* Vector to store all the labels in the .data and .text section */
std::vector<Label> labels;

/* Data types supported in the .data section */
std::vector<std::string> data_types {

    ".ascii", ".asciiz", ".word", ".byte", ".half"

};

/* Map that maps R - instructions in the format {instruction, function_code} */
std::map<std::string, std::string> R_Instructions {

    {"add", "100000"}, {"addu", "100001"}, {"and", "100100"}, {"div", "011010"}, {"divu", "011011"},
    {"jalr", "001001"}, {"jr", "001000"}, {"mfhi", "010000"}, {"mflo", "010010"}, {"mthi", "010001"},
    {"mtlo", "010011"}, {"mult", "011000"}, {"multu", "011001"}, {"nor", "100111"}, {"or", "100101"},
    {"sll", "000000"}, {"sllv", "000100"}, {"slt", "101010"}, {"sltu", "101011"}, {"sra", "000011"},
    {"srav", "000111"}, {"srl", "000010"}, {"srlv", "000110"}, {"sub", "100010"}, {"subu", "100011"},
    {"syscall", "001100"}, {"xor", "100110"}, {"clo", "100001"}, {"clz", "100000"}, {"mul", "000010"},
    {"madd", "000000"}, {"maddu", "000001"}, {"msub", "000100"}, {"msubu", "000101"}, {"tlt", "110010"},
    {"tltu", "110011"}, {"tge", "110000"}, {"tgeu", "110001"}, {"tne", "110110"}, {"teq", "110100"}

};

/* Map that maps I - instructions in the format {instruction, op_code} */
std::map<std::string, std::string> I_Instructions {

    {"addi", "001000"}, {"addiu", "001001"}, {"andi", "001100"}, {"beq", "000100"}, {"bgez", "000001"},
    {"bgezal", "000001"}, {"bgtz", "000111"}, {"blez", "000110"}, {"bltz", "000001"}, {"bne", "000101"}, 
    {"lb", "100000"}, {"lbu", "100100"}, {"lh", "100001"}, {"lhu", "100101"}, {"lui", "001111"}, 
    {"lw", "100011"}, {"ori", "001101"}, {"sb", "101000"}, {"slti", "001010"}, {"sc", "111000"},
    {"sltiu", "001011"}, {"sh", "101001"}, {"sw", "101011"}, {"xori", "001110"}, {"tlti", "000001"},
    {"bltzal", "000001"}, {"swr", "101110"}, {"swl", "101010"}, {"ll", "110000"}, {"tltiu", "000001"},
    {"lwr", "100110"}, {"lwl", "100010"}, {"tgei", "000001"}, {"tgeiu", "000001"}, {"tnei", "000001"},
    {"teqi", "000001"}

};

/* Map that maps J - instructions in the format {instruction, op_code} */
std::map<std::string, std::string> J_Instructions {
    
    {"j", "000010"}, {"jal", "000011"}
    
};

/* Map that maps J - instructions in the format {instruction, op_code} */
std::map<std::string, int> registers = {

    {"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3}, {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}

};

#endif