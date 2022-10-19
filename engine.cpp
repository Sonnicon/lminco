#include <string>
#include <fstream>
#include <iostream>
#include "main.h"
#include "engine.h"


#define CLAMP(x, l, h) (x+(l>x)*(l-x)+(x>h)*(h-x))
#define GETMEMORY(address) (memory[2*address]|(memory[2*address+1]<<8))

#define VERBOSE_OP (" op=" + std::to_string(opcode))
#define VERBOSE_PC (" pc=" + std::to_string(programCounter))
#define VERBOSE_AC (" ac=" + std::to_string(accumulator))

Engine::~Engine() {
    free(memory);
}

void Engine::load(std::string *file) {
    std::ifstream stream(*file, std::ios::binary);
    unsigned char c;
    for (auto i = 0; i < 200; i++) {
        stream.read(reinterpret_cast<char *>(&c), 1);
        memory[i] = c & (0xFF - 0xC0 * (i & 1));
    }
}

void Engine::loadStdin() {
    fread(memory, 1, 200, stdin);

}

void Engine::run() {
    while (execute()) {}

    std::cout << "Output Summary:" << std::endl;
    while (!outputs.empty()) {
        std::cout << "Output: " << outputs.front() << std::endl;
        outputs.pop();
    }
}

bool Engine::execute() {
    if (programCounter > 99 || programCounter < 0) {
        std::cout << "Error: pc is out of bounds of memory; exiting." << VERBOSE_PC << std::endl;
        exit(1);
    }
    unsigned short opcode = GETMEMORY(programCounter);
    unsigned char opcodeI = opcode / 100;
    unsigned char opcodeA = opcode % 100;

    if (flags & FLAG_VERBOSE) {
        std::cout << "Executing:" << VERBOSE_OP << VERBOSE_PC << VERBOSE_AC << std::endl;
    }
    programCounter++;

    switch (opcodeI) {
        case (0):
            op_hlt();
            return false;
        case (1):
            op_add(opcodeA);
            break;
        case (2):
            op_sub(opcodeA);
            break;
        case (3):
            op_sto(opcodeA);
            break;
        case (5):
            op_lda(opcodeA);
            break;
        case (6):
            op_br(opcodeA);
            break;
        case (7):
            op_brz(opcodeA);
        case (8):
            op_brp(opcodeA);
        case (9):
            if (opcodeA & 1) {
                op_in();
            } else {
                op_out();
            }
            break;
        default:
            std::cout << "Warning: Invalid Instruction" << VERBOSE_OP << VERBOSE_PC << std::endl;
            break;
    }
    return true;
}

void Engine::op_add(unsigned char address) {
    unsigned short value = GETMEMORY(address);
    if (accumulator + value < 1000) {
        accumulator += value;
    } else {
        std::cout << "Warning: undocumented add wrap triggered." << VERBOSE_PC << std::endl;
        accumulator = (accumulator + value) % 1000;
    }
}

void Engine::op_sub(unsigned char address) {
    unsigned short value = GETMEMORY(address);
    if (accumulator >= value) {
        accumulator -= value;
    } else {
        std::cout << "Warning: undocumented sub wrap triggered." << VERBOSE_PC << std::endl;
        accumulator += 1000 - value;
        accumulatorFlag = true;
    }
}

void Engine::op_sto(unsigned char address) {
    memory[2 * address] = accumulator & 0xFF;
    memory[2 * address + 1] = (accumulator >> 8) & 3;
}

void Engine::op_lda(unsigned char address) {
    accumulator = memory[2 * address + 1];
    accumulator |= memory[2 * address] << 8;
    accumulatorFlag = false;
}

void Engine::op_br(unsigned char address) {
    programCounter = GETMEMORY(address);
}

void Engine::op_brz(unsigned char address) {
    if (accumulator == 0) {
        programCounter = GETMEMORY(address);
    }
}

void Engine::op_brp(unsigned char address) {
    if (!accumulatorFlag) {
        programCounter = GETMEMORY(address);
    }
}

void Engine::op_in() {
    unsigned short value;
    if (inputs.empty()) {
        std::cout << "Enter input: ";
        std::cin >> value;
        std::cout << std::endl;
    } else {
        value = inputs.front();
        inputs.pop();
    }
    accumulator = CLAMP(value, 0, 999);
}

void Engine::op_out() {
    outputs.push(accumulator);
    std::cout << "New Output: " << std::to_string(accumulator) << std::endl;
}

void Engine::op_hlt() {
    std::cout << "Halted." << std::endl;
}
