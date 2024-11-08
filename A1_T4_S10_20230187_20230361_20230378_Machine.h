#ifndef MACHINE_H
#define MACHINE_H

#include "A1_T4_S10_20230187_20230361_20230378_Register.h"
#include "A1_T4_S10_20230187_20230361_20230378_Memory.h"
#include "A1_T4_S10_20230187_20230361_20230378_Instruction.h"
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <memory>

using namespace std;

class Machine {
private:
    Register registers;
    Memory memory;
    uint8_t PC = 0; 
    uint16_t IR = 0; 

public:
    void loadProgram(const string& filename);
    void runStep();
    void runUntilHalt();
    void displayState() const;
    void menu();
};

#endif // MACHINE_H
