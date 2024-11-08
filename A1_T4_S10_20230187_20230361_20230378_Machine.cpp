#include "A1_T4_S10_20230187_20230361_20230378_Machine.h"


void Machine::loadProgram(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int address;
        uint16_t instruction;
        ss >> hex >> address >> instruction; 

        if (address >= 0 && address < MEMORY_SIZE) {
            memory[address] = instruction; 
        } else {
            cerr << "Warning: Address " << hex << address << " is out of bounds.\n";
        }
    }
    file.close();
}

void Machine::runStep() {
    bool running = true;
    while (running && PC < MEMORY_SIZE - 1) { 
        IR = (memory[PC] << 8) | memory[PC + 1]; 
        PC += 2; 
        Instruction::execute(IR, registers, memory, PC, running);
        displayState();
        cout << "Press Enter to continue or type 'q' to go back to the menu: ";
        string input;
        getline(cin, input);
        if (input == "q") {
            running = false; 
        }
    }
}

void Machine::runUntilHalt() {
    bool running = true;
    while (running && PC < MEMORY_SIZE - 1) {
        IR = (memory[PC] << 8) | memory[PC + 1];
        PC += 2;
        Instruction::execute(IR, registers, memory, PC, running);
        displayState();
    }
}

void Machine::displayState() const {
    cout << "\nPC: " << setw(2) << setfill('0') << hex << static_cast<int>(PC) << " ";
    cout << "IR: " << setw(4) << setfill('0') << hex << IR << endl;
    registers.display();
    memory.display();
}

void Machine::menu() {
    string command;
    while (true) {
        cout << "Vole Machine Menu:\n";
        cout << "1. Load Program\n";
        cout << "2. Run Step\n";
        cout << "3. Run Until Halt\n";
        cout << "4. Display State\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        getline(cin, command);

        if (command == "1") {
            cout << "Enter program filename: ";
            string filename;
            getline(cin, filename);
            loadProgram(filename);
        } else if (command == "2") {
            runStep();
        } else if (command == "3") {
            runUntilHalt();
        } else if (command == "4") {
            displayState();
        } else if (command == "5") {
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
