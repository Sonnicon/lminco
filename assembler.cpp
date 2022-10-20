#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include "main.h"
#include "assembler.h"

const std::map<std::string, unsigned short> Assembler::mnemonics {
        {"ADD", 100}, {"SUB", 200}, {"STO", 300},
        {"LDA", 500}, {"BR", 600}, {"BRZ", 700},
        {"BRP", 800}, {"IN", 901}, {"OUT", 902},
        {"HLT", 000}, {"DAT", 000}};

const std::set<std::string> Assembler::mnemonicsNodata {"IN", "OUT", "HLT"};

struct Assembler::Command {
    unsigned short instruction;
    std::string data;
};

std::string* Assembler::assembleFile(std::string* ifile, std::string* ofile) {
    std::ifstream in(*ifile);
    std::string line;
    unsigned char lineNum = 0;
    std::map<std::string, unsigned char> labelMap{{"", 0}};
    std::vector<Assembler::Command*> commands{};

    while (std::getline(in, line) && lineNum < 100) {
        auto firstIndex = line.find_first_not_of("\t ");
        if (line.empty() || line[firstIndex] == '#') {
            if (flags & FLAG_VERBOSE) {
                std::cout << "Skipping line:" << std::to_string(lineNum) << std::endl;
            }
            continue;
        }

        std::stringstream split(line);
        std::string label;
        if (firstIndex == 0) {
            split >> label;
        } else {
            label = "_" + std::to_string(lineNum);
        }

        std::string inStr;
        split >> inStr;
        if (inStr == "-") {
            if (flags & FLAG_VERBOSE) {
                std::cout << "No instruction line:" << std::to_string(lineNum) << std::endl;
            }
            continue;

        }
        auto ins = mnemonics.find(inStr);
        if (ins == mnemonics.end()) {
            std::cout << "Critical Error: Bad instruction line:" << std::to_string(lineNum) << std::endl;
            exit(1);
        }

        auto* currentCommand = new Command();
        currentCommand->instruction = ins -> second;
        if (mnemonicsNodata.find(inStr) == mnemonicsNodata.end()) {
            if (split.eof() && inStr != "DAT") {
                std::cout << "Critical Error: Instruction missing data:" << std::to_string(lineNum) << std::endl;
                exit(1);
            }
            split >> currentCommand->data;
        }

        commands.push_back(currentCommand);
        labelMap[label] = lineNum++;
    }

    std::ofstream out(*ofile, std::ios::binary);
    for (Assembler::Command* & cmd : commands) {
        unsigned short i = cmd->instruction;
        if (!cmd->data.empty()) {
            if (cmd->data.find_first_not_of("0123456789") == std::string::npos) {
                i += std::stoi(cmd->data);
            } else {
                auto instr = labelMap.find(cmd->data);
                if (instr == labelMap.end() && cmd->instruction != 0) {
                    std::cout << "Critical Error: Undefined label referenced:" << cmd->data << std::endl;
                    exit(1);
                }
                i += instr->second;
            }
        }
        out.write((char*) &i, 2);
        free(cmd);
    }
    out.close();

    return nullptr;
}
