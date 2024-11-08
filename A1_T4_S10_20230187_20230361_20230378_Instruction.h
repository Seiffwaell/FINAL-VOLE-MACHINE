#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <memory>
#include "A1_T4_S10_20230187_20230361_20230378_Register.h"
#include "A1_T4_S10_20230187_20230361_20230378_Memory.h"

using namespace std;

class Instruction {
public:
    static void execute(uint16_t instruction, Register& reg, Memory& mem, uint8_t& PC, bool& running);
};

#endif // INSTRUCTION_H
