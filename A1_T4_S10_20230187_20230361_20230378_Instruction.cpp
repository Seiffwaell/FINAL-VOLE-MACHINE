#include "A1_T4_S10_20230187_20230361_20230378_Instruction.h"
#include <iomanip>

void Instruction::execute(uint16_t instruction, Register& reg, Memory& mem, uint8_t& PC, bool& running) {
    uint8_t opCode = (instruction >> 12) & 0xF; 
    uint8_t R, S, XY, T;

    if (opCode == 4) {
        R = (instruction >> 4) & 0xF;       
        S = instruction & 0xF;              
    } else {
        R = (instruction >> 8) & 0xF;       
        S = (instruction >> 4) & 0xF;       
        XY = instruction & 0xFF;            
        T = instruction & 0xF;
    }

    switch (opCode) {
        case 1: 
            if (XY < MEMORY_SIZE) {
                reg[R] = mem[XY] & 0x0FFF; 
            } else {
                cerr << "Error: Memory address " << hex << XY << " out of bounds." << endl;
            }
            break;
        case 2: 
            reg[R] = XY;
            break;
        case 3: 
            mem[XY] = reg[R];
            if (XY == 0x00) {
                cout << "\nScreen Output(ASCII): " << static_cast<char>(reg[R]) << endl; 
                cout << "\nScreen Output(HEX): " << reg[R] << endl; 
            break;
        case 4: 
            if (R < REGISTER_COUNT && S < REGISTER_COUNT) {
                reg[S] = reg[R]; 
            } else {
                cerr << "Error: Invalid register index." << std::endl;
            }
            break;
        case 5: 
        {
            int16_t sValue = static_cast<int16_t>(reg[S]);
            int16_t tValue = static_cast<int16_t>(reg[T]);
            int16_t result = sValue + tValue;
            reg[R] = static_cast<uint16_t>(result) & 0xFF; 
        }
        break;
        case 6: 
        {
            uint16_t sValue = reg[S];
            uint16_t tValue = reg[T];
            bool s_signbit = (sValue >> 7) & 1;
            int s_exponent_excess = (sValue >> 4) & 0x7; // Exponent with bias of 4
            int s_mantissa = sValue & 0xF; // Mantissa
            bool t_signbit = (tValue >> 7) & 1;
            int t_exponent_excess = (tValue >> 4) & 0x7; // Exponent with bias of 4
            int t_mantissa = tValue & 0xF; // Mantissa
            int s_fixed = (1 - 2 * s_signbit) * (s_mantissa << s_exponent_excess);
            int t_fixed = (1 - 2 * t_signbit) * (t_mantissa << t_exponent_excess);
            int r_fixed = s_fixed + t_fixed;
            bool r_signbit = r_fixed < 0;
            int r_mantissa_fixed = r_signbit ? -r_fixed : r_fixed;

            if (r_mantissa_fixed == 0) {
                reg[R] = 0;
                break;
            }

            auto normalise = [](int f, int e_excess) {
                while (e_excess >= 0) {
                    int p = (f >> e_excess) & 0xF;
                    if ((p & 0x8) != 0 || e_excess == 0)
                        return std::make_pair(p, e_excess);
                    e_excess--;
                }
                return std::make_pair(0, -1);
            };

            auto [r_mantissa, r_exp_excess] = normalise(r_mantissa_fixed, 8);

            if (r_exp_excess > 7) {
                reg[R] = (r_signbit << 7) | (7 << 4) | 0xF;
            } else {
                reg[R] = (r_signbit << 7) | (r_exp_excess << 4) | r_mantissa;
            }
        }
        break;
        case 7: 
            reg[R] = reg[S] | reg[T];
            break;
        case 8: 
            reg[R] = reg[S] & reg[T];
            break;
        case 9: 
            reg[R] = reg[S] ^ reg[T];
            break;
        case 0xA: 
        {
            uint8_t X = instruction & 0xF;
            reg[R] = (reg[R] >> X) | (reg[R] << (16 - X));
        }
        break;
        case 0xB: 
            if (reg[R] == reg[0]) {
                PC = XY;
            }
            break;
        case 0xC: 
            cout << "Execution halted.\n";
            running = false;
            break;
        default:
            cout << "Invalid instruction!\n";
            break;
        }
    }
}