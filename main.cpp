#include "utils.h"
#include <sstream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <typeinfo>

using std::cout;
using std::endl;
using std::string;
using std::bitset;
using std::stringstream;
using std::vector;
using std::map;
using std::to_string;
using std::ifstream;
using std::ostream;
using std::streampos;
using std::ofstream;

/* Overloading Functions */
bool operator==(Label label, string string) {return label.getName() == string;}
bool operator==(string string, Label label) {return label.getName() == string;}
ostream &operator<<(ostream &os, Label label) {return os << label.getName() << endl << label.getContent();}

/* Looks for the address of the label from the label list */
int labelToInstrAddr(string label) {
    for (auto &elem: labels)
        if (label == elem) return elem.getAddress();
    
    return -1;
}

/* Changes the register to its corresponding 5-bit address */
string regTrans(string reg) {
    string result;

    for (char const &c: reg) {
        if (isdigit(c) != 0) 
            return reg;

        else break;
    }

    auto it = registers.find(reg);

    if (it != registers.end()) {
        result = bitset<5>(it->second).to_string();
    
        return result;

    } else return "";
}

/* Helper function that changes a string to an int */
int transReg(string reg) {
    int reg_index;
    reg_index = stoi(reg);

    return reg_index;
}

/* Get rid of any spaces or tabs at the start or at the end of a line */
string trim(string line) {
    string formatted_line;

    if (line.empty()) return "";

    if (line.at(0) == '\t' || line.at(0) == ' ') 
        formatted_line = line.substr(1);

    else formatted_line = line;

    if (formatted_line.back() == '\t' || formatted_line.back() == ' ') 
        formatted_line = formatted_line.substr(0, line.size() - 1);

    return formatted_line;
}

/* Remove the comments in the file */
stringstream removeComments(ifstream &is) {
    string line; //The line variable which will be used as the temporary memory
    stringstream out; //Stringstream variable that will be output

    /* The main algorithm that will delete comments */
    while (getline(is, line)) {
        int delimiter = line.find('#');

        if (delimiter != -1) 
            line.erase(delimiter);

        out << line << endl;
    }
    
    return out;
}

/* Function that assembles an R - type instruction in machine code */
string makeR_type(string instruction, string rd, string rs, string rt, string shamt, string funct) {
    string op = "000000";
    string dest_reg = regTrans(rd);
    string first_reg = regTrans(rs);
    string second_reg = regTrans(rt);

    int temp = stoi(shamt);
    string shift_amt = bitset<5>(temp).to_string();

    if (first_reg.empty()) first_reg = "00000";
    
    if (second_reg.empty()) second_reg = "00000";
 
    if (dest_reg.empty()) dest_reg = "00000";

    if (instruction == "clo" || instruction == "clz" || instruction == "mul" || 
       instruction == "madd" || instruction == "maddu" || instruction == "msub" ||
       instruction == "msubu") op = "011100";

    return op + first_reg + second_reg + dest_reg + shift_amt + funct;
}

/* Function that assembles an I - type instruction in machine code */
string makeI_type(string instruction, string op, string rs, string rt, string immediate) {
    string first_reg = regTrans(rs);
    string dest_reg = regTrans(rt);
    int temp = stoi(immediate);
    string imm_val = bitset<16>(temp).to_string();

    if (first_reg.empty()) first_reg = "00000";

    if (dest_reg.empty()) dest_reg = "00000";
    
    return op + first_reg + dest_reg + imm_val;
}

/* Function that assembles an I - type instruction in machine code */
string makeJ_type(string op, string address) {
    int temp = stoi(address);
    string address_val = bitset<26>(temp).to_string();

    return op + address_val;
}

void makeInstruction(stringstream &result, vector<string> &tokens, int &PC) {
    map<string, string>::iterator it;

    if (tokens[0] == "add") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "addu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "addi") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "addiu") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "and") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "andi") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "clo") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], "", "0", it->second) << endl;
    } else if (tokens[0] == "clz") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], "", "0", it->second) << endl;
    } else if (tokens[0] == "div") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "divu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "mult") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "multu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "mul") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "madd") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "msub") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "maddu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "msubu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "nor") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "or") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "ori") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "sll") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], "", tokens[2], tokens[3], it->second) << endl;
    } else if (tokens[0] == "sllv") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[3], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "sra") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], "", tokens[2], tokens[3], it->second) << endl;
    } else if (tokens[0] == "srav") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[3], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "srl") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], "", tokens[2], tokens[3], it->second) << endl;
    } else if (tokens[0] == "srlv") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[3], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "sub") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "subu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "xor") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "xori") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "lui") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, "00000", tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "slt") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "sltu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], tokens[3], "0", it->second) << endl;
    } else if (tokens[0] == "slti") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "sltiu") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[2], tokens[1], tokens[3]) << endl;
    } else if (tokens[0] == "beq") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[3]);
        if (temp == -1)
            result << makeI_type(tokens[0], it->second, tokens[1], tokens[2], tokens[3]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], tokens[2], to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bgez") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[2]);
        if (temp == -1)   
            result << makeI_type(tokens[0], it->second, tokens[1], "00001", tokens[2]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "00001", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bgezal") {
        it = I_Instructions.find(tokens[0]);
        int32_t temp = labelToInstrAddr(tokens[2]);
        if (temp == -1) 
            result << makeI_type(tokens[0], it->second, tokens[1], "10001", tokens[2]) << endl;
        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "10001", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bgtz") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[2]);
        if (temp == -1)   
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", tokens[2]) << endl;
        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "blez") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[2]);
        if (temp == -1)
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", tokens[2]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bltzal") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[2]);
        if (temp == -1) 
            result << makeI_type(tokens[0], it->second, tokens[1], "10000", tokens[2]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "10000", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bltz") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[2]);
        if (temp == -1)  
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", tokens[2]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], "00000", to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "bne") {
        it = I_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[3]);
        if (temp == -1)
            result << makeI_type(tokens[0], it->second, tokens[1], tokens[2], tokens[3]) << endl;

        else {
            int relative_addr = temp - (0x400000 + ((PC *4) + 4));
            result << makeI_type(tokens[0], it->second, tokens[1], tokens[2], to_string(relative_addr/4)) << endl;
        }

    } else if (tokens[0] == "j") {
        it = J_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[1]);
        if (temp == -1)
            result << makeJ_type(it->second, tokens[1]) << endl;
        else 
            result <<makeJ_type(it->second, to_string(temp/4)) << endl;

    } else if (tokens[0] == "jal") {
        it = J_Instructions.find(tokens[0]);
        int temp = labelToInstrAddr(tokens[1]);
        if (temp == -1) 
            result << makeJ_type(it->second, tokens[1]) << endl;
        else
            result <<makeJ_type(it->second, to_string(temp>>2)) << endl;

    } else if (tokens[0] == "jalr") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], tokens[2], "", "0", it->second) << endl;
    } else if (tokens[0] == "jr") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], "", "0", it->second) << endl;
    } else if (tokens[0] == "teq") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "teqi") { 
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01100", tokens[2]) << endl;
    } else if (tokens[0] == "tne") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "tnei") { 
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01110", tokens[2]) << endl;
    } else if (tokens[0] == "tge") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "tgeu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "tgei") { 
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01000", tokens[2]) << endl;
    } else if (tokens[0] == "tgeiu") { 
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01001", tokens[2]) << endl;
    } else if (tokens[0] == "tlt") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "tltu") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], tokens[2], "0", it->second) << endl;
    } else if (tokens[0] == "tlti") { 
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01010", tokens[2]) << endl;
    } else if (tokens[0] == "tltiu") {
        it = I_Instructions.find(tokens[0]);
        result << makeI_type(tokens[0], it->second, tokens[1], "01011", tokens[2]) << endl;
    } else if (tokens[0] == "lb") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lbu") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lh") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lhu") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lw") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lwl") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "lwr") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "ll") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "sb") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "sh") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "sw") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "swl") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "swr") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "sc") {
        it = I_Instructions.find(tokens[0]);
        size_t open_bracket = tokens[2].find('(');
        size_t close_bracket = tokens[2].find(')');
        string rs = tokens[2].substr(open_bracket + 1, close_bracket - (open_bracket + 1));
        result << makeI_type(tokens[0], it->second, rs, tokens[1], tokens[2]) << endl;
    } else if (tokens[0] == "mfhi") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], "", "", "0", it->second) << endl;
    } else if (tokens[0] == "mflo") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], tokens[1], "", "", "0", it->second) << endl;
    } else if (tokens[0] == "mthi") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], "", "0", it->second) << endl;
    } else if (tokens[0] == "mtlo") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", tokens[1], "", "0", it->second) << endl;
    } else if (tokens[0] == "syscall") {
        it = R_Instructions.find(tokens[0]);
        result << makeR_type(tokens[0], "", "", "", "0", it->second) << endl;
    } 

    PC++;
    tokens.clear();
}

/* The first parsing function that will store the labels */
void firstParse(string iss) {
    int data_count = 0; //This variable records the program counter in the .data section
    int instruction_count = 0; //This variable records the program counter in the .text section
    
    string line; //String variable that reads each line
    string formatted_line; //String variable that stores the trimmed line
    stringstream is(iss); //Conversion to stringstream

    /* While loop that will look for the .data segment */
    while (getline(is, line)) {
        formatted_line = trim(line);
        if (formatted_line == ".data") break;
    }

    /* Once the .data segment is found, store the labels */
    while (getline(is, line)) {
        formatted_line = trim(line);
        
        if (formatted_line == ".text") break; // When ".text" is found

        if (formatted_line.empty()) continue;
            
        /* Checking if any of the data types (Ex: .ascii) is contained within the line */
        for (const auto &data_type: data_types) {
            
            size_t delimiter = formatted_line.find(':');
            string temp = formatted_line.substr(delimiter + 1);
            string formatted_temp = trim(temp);
            size_t delimiter2 = formatted_temp.find(' ');
            string type_of_data = formatted_temp.substr(0, delimiter2);

            if (type_of_data == data_type) {
                //Split the line to only hold the label and store it in the labels
                //vector along with its corresponding address
                string temp2 = formatted_temp.substr(delimiter2 + 1);

                if (temp2.find('"') != string::npos) 
                    temp2.erase(remove(temp2.begin(), temp2.end(), '"'), temp2.end());
                
                Label newLabel(formatted_line.substr(0, delimiter), 0x500000 + (data_count) * 4, data_type, temp2);

                data_count++; //Increase the counter

                labels.push_back(newLabel);

            }
        }
    }

    /* Checks if it has reached the end of the file */
    streampos old_pos = is.tellg();
    
    if (!getline(is, line)) {
        is.clear();
        is.seekg(0, std::ios::beg);

        while (getline(is, line)) {
            formatted_line = trim(line);
            if (formatted_line == ".text") break;
        }

    } else is.seekg(old_pos); //Move it back one line
    
    /* Parsing the .text segment */
    while (getline(is,line)) {
        formatted_line = trim(line);

        if (formatted_line.empty()) continue;

        /* If a label is found */
        if (formatted_line.find(':') != string::npos) {
            bool format = true; //State variable to store the formatting style
                                //true for label: instruction format
                                //false for label:
                                //                 instruction format

            //Look for the space after the ':' (belongs to the label) in the line
            //If it is not found then it follows the false-style formatting
            if (formatted_line.find(' ') == string::npos) format = false;
            
            //Split the line to only hold the label and store it in the labels
            //vector along with its corresponding address
            size_t delimiter = formatted_line.find(':');
            Label newLabel(formatted_line.substr(0, delimiter), 0x400000 + (instruction_count * 4));

            //Increase the counter only if it follows the true-style formatting
            if (format == true) instruction_count++;

            labels.push_back(newLabel);
        } 
        
        //If it encounters the .data segment again
        else if (formatted_line == ".data") break;

        //If it is not a label, then it is an instruction
        else instruction_count++;    
    }
}   

/* The second parsing function that will assemble instructions */
stringstream secondParse(stringstream &is) {
    int PC = 0;
    string line;
    string formatted_line;
    stringstream result;

    /* Looking for the .text segment */
    while (getline(is, line)) {
        formatted_line = trim(line);
        if (formatted_line == ".text") break;
    }

    /* Assembling the .text segment */
    while (getline(is, line)) {
        formatted_line = trim(line);

        if (formatted_line.empty()) continue;

        /* Ignore the labels */
        if (formatted_line.find(": ") != string::npos) { //For false-style formatting
            size_t delimiter = formatted_line.find(' ');
            formatted_line = formatted_line.substr(delimiter + 1);

        } else if (formatted_line.find(':') != string::npos) continue; //For true-style formatting

        /* Assembling the instructions */
        vector<string> tokens; //Vector to store the tokens
        stringstream ss(formatted_line); //Change formatted_line into a stringstream
        string intermediate; //Intermediate string variable to hold the tokens

        /* Tokenize the line */
        while (getline(ss, intermediate, ' ')) {
            if (intermediate.empty()) continue;

            /* Remove the commas at the end and store them in the tokens vector */
            if (intermediate.back() == ',')
                tokens.push_back(intermediate.substr(0, intermediate.size() - 1));

            else tokens.push_back(intermediate);
        }

        makeInstruction(result, tokens, PC);
    }

    return result;
}

/* Main assembling function */
stringstream assemble(string filename) {
    ifstream infile;
    stringstream result;
    infile.open(filename);
    if (infile.is_open()) {
        stringstream formatted_file = removeComments(infile);
        string no_comments = formatted_file.str();
        firstParse(no_comments);

        result = secondParse(formatted_file);
    }

    infile.close();
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error, wrong number of inputs" << endl;
        exit(1);
    }

    string filename(argv[1]);
    string output(argv[2]);
    
    stringstream binary_instructions = assemble(filename);
    ofstream outfile(output);

    outfile << binary_instructions.str() << endl;

    outfile.close();
}