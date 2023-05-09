map<ui, int> MEM;
map<ui, string> logs;
map<string, int> regToId;
map<string, ui> labToAdd;
bitset<32> to_bin;
int REG[32];
ll program_counter;
ifstream curFile;

vector<string> csv_to_vec(string s);


void assert_label(string l) {
    if(!l.size()) quit();
    if(!labToAdd.count(l)) {
        cout << "LABEL " << l <<  " DOESN'T EXIST\n";
        quit();
    }
}
void add(int rd, int rs1, int rs2) {
    if (!rd) return;
    else REG[rd] = REG[rs1] + REG[rs2];
    program_counter += 4;
}
void sub(int rd, int rs1, int rs2) {
    if (!rd) return;
    else REG[rd] = REG[rs1] - REG[rs2];
    program_counter += 4;
}

void _or(int rd, int rs1, int rs2) {
    if (!rd) return;
    else REG[rd] = REG[rs1] | REG[rs2];
    program_counter += 4;
}

void _and(int rd, int rs1, int rs2) {
    if (rd == 0) return;
    else REG[rd] = REG[rs1] & REG[rs2];
    program_counter += 4;
}

void _xor(int rd, int rs1, int rs2) {
    if (!rd) return;
    else REG[rd] = REG[rs1] ^ REG[rs2];
    program_counter += 4;
}
void sll(int rd, int rs1, int rs2) {
    if (!rd) return;
    else if (REG[rs2] > 31 | REG[rs2] < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SLL. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] << REG[rs2];
    program_counter += 4;
}
void srl(int rd, int rs1, int rs2) {
    if (!rd) return;
    else if (REG[rs2] > 31 | REG[rs2] < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SRL. TERMINATED!" << endl;
        quit();
    } else REG[rd] = (ui) REG[rs1] >> REG[rs2];
    program_counter += 4;
}

void sra(int rd, int rs1, int rs2) {
    if (!rd) return;
    else if (REG[rs2] > 31 | REG[rs2] < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SRA. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] >> REG[rs2];
    program_counter += 4;
}

void slt(int rd, int rs1, int rs2) {
    if (!rd) return;
    if (REG[rs1] < REG[rs2]) REG[rd] = 1;
    else REG[rd] = 0;
    program_counter += 4;
}

void sltu(int rd, int rs1, int rs2) {
    if (!rd) return;
    if ((ui) REG[rs1] < (ui) REG[rs2]) REG[rd] = 1;
    else REG[rd] = 0;
    program_counter += 4;
}
void addi(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - ADDI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] + imm;
    program_counter += 4;
}
void andi(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - ANDI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] & imm;
    program_counter += 4;
}

void ori(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - ORI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] | imm;
    program_counter += 4;
}

void xori(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - XORI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] ^ imm;
    program_counter += 4;
}

void slli(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (imm > 31 || imm < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SLLI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] << imm;
    program_counter += 4;
}

void srli(int rd, int rs1, int imm) {
    if (!rd) return;
    else if (imm > 31 || imm < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SRLI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = (ui) REG[rs1] >> imm;
    program_counter += 4;
}

void srai(int rd, int rs1, int imm) 
{
    if (!rd) return;
    else if (imm > 31 | imm < 0) {
        cout << "ERROR: SHIFTING MORE THAN 31 BIT - SRAI. TERMINATED!" << endl;
        quit();
    } else REG[rd] = REG[rs1] >> imm;
    program_counter += 4;
}

void jalr(int rd, int rs1, int imm) {
    if (rd != 0) REG[rd] = program_counter + 4;
    if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 12 BIT - JALR. TERMINATED!" << endl;
        quit();
    } else program_counter = REG[rs1] + imm;
}
void lw(int rd, int rs1, int imm) {
    if (!rd) return;
    if (!good_imm(imm)) {
        cout << "ERROR: SHIFTING MORE THAN 12 BIT - LW. TERMINATED!"<< endl;
        quit();
    }
    int address = REG[rs1] + imm;
    if (MEM.find(address) != MEM.end()) REG[rd] = MEM[address];
    else {
        cout << "ERROR: LOADING FROM A NON-ALLOCATED ADDRESS - LW. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void lh(int rd, int rs1, int imm) {
    if (!rd) return;
    if (good_imm(imm)) {
        int r = (REG[rs1] + imm) % 4;
        int address = REG[rs1] + imm - r;
        if (MEM.find(address) != MEM.end()) REG[rd] = (MEM[address] << (8 * (2 - r))) >> 16;
        else {
            cout << "ERROR: INPUTTING FROM A NON-ALLOCATED ADDRESS - LH. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else { 
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - LH. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void lb(int rd, int rs1, int imm) {
    if (!rd) return;
    if (good_imm(imm)) {
        int r = (REG[rs1] + imm) % 4;
        int address = REG[rs1] + imm - r;

        if (MEM.find(address) != MEM.end()) REG[rd] = (MEM[address] << (8 * (3 - r))) >> 24;
        else {
            cout << "ERROR: LOADING FROM A NON-ALLOCATED ADDRESS - LB. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else {
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - LB. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void lhu(int rd, int rs1, int imm) {
    if (!rd) return;
    if (good_imm(imm)) {
        int r = (REG[rs1] + imm) % 4;
        int address = REG[rs1] + imm - r;
        if (MEM.find(address) != MEM.end()) {
            REG[rd] = (ui)(MEM[address] << (8 * (2 - r))) >> 16;
        } else {
            cout << "ERROR: LOADING FROM A NON-ALLOCATED ADDRESS - LHU. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else {
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - LB. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void lbu(int rd, int rs1, int imm) {
    if (!rd) return;
    if (good_imm(imm)) {
        int r = (REG[rs1] + imm) % 4;
        int address = REG[rs1] + imm - r;
        if (MEM.find(address) != MEM.end()) REG[rd] = (ui)(MEM[address] << (8 * (3 - r))) >> 24;
        else {
            cout << "ERROR: LOADING FROM A NON-ALLOCATED ADDRESS - LBU. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else {
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - LBU. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void slti(int rd, int rs1, int imm) {
    if (!rd) return;
    if (!good_imm(imm)) {
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - SLTI. TERMINATED!" << endl;
        quit();
    } else (REG[rs1] < imm ? REG[rd] = 1 : REG[rd] = 0);
    program_counter += 4;
}

void sltiu(int rd, int rs1, int imm) {
    if (!rd) return;
    if (!good_imm(imm)) {
        cout << "ERROR: INPUTTING MORE THAN 12 BITS IN IMMEDIATE - SLTUI. TERMINATED!" << endl;
        quit();
    } else ((ui)REG[rs1] < (ui) imm ? REG[rd] = 1 : REG[rd] = 0);
    program_counter += 4;
}
void sw(int rs1, int rs2, int imm) {
    int final_address = REG[rs2] + imm;
    if (final_address % 4) {
        cout << "STORING A WORD NON-DIVASABLE BY FOUR. TERMINATED!" << endl;
        quit();
    } else MEM[final_address] = REG[rs1];
    program_counter += 4;
}

void sh(int rs1, int rs2, int imm) {
    int address = REG[rs2] + imm, r = address % 4, final_address = address - r;
    if (final_address % 2) {
        cout << "STORING HALF A WORD NON-DIVASABLE BY TWO. TERMINATED!" << endl;
        quit();
    } else {
        ui half = REG[rs1] << 16;
        half = half >> 16;
        half = half << 8 * r;
        ui mask = HALFREP;
        mask = mask << 8 * r;
        mask = ~mask;
        int edited_word = MEM[final_address];
        edited_word = ((edited_word & mask) | half);
        MEM[final_address] = edited_word;
    }
    program_counter += 4;
}

void sb(int rs1, int rs2, int imm) {
    int address = REG[rs2] + imm;
    int r = address % 4;
    int final_address = address - r;

    ui byte = REG[rs1] << 24;
    byte = byte >> 24;
    byte = byte << 8 * r;

    ui mask = BYTREP;
    mask = mask << 8 * r;
    mask = ~mask;
    int edited_word = MEM[final_address];
    edited_word = ((edited_word & mask) | byte);

    MEM[final_address] = edited_word;
    program_counter += 4;
}

void beq(int rs1, int rs2, string label) {
    assert_label(label);
    (REG[rs1] == REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}

void beq(int rs1, int rs2, int offset) {
    if (REG[rs1] == REG[rs2]) {
        if (good_imm(offset)) program_counter = program_counter + 2 * offset;
        else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BEQ. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}

void bne(int rs1, int rs2, string label) {
    assert_label(label);
    (REG[rs1] != REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}
void bne(int rs1, int rs2, int offset) {
    if (REG[rs1] != REG[rs2]) {
        if (good_imm(offset)) program_counter = program_counter + 2 * offset;
        else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BNE. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}

void blt(int rs1, int rs2, string label) {
    assert_label(label);
    (REG[rs1] < REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}
void blt(int rs1, int rs2, int offset) {
    if (REG[rs1] < REG[rs2]) {
        if (good_imm(offset)) {
            program_counter = program_counter + 2 * offset;
        } else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BLT. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}

void bltu(int rs1, int rs2, string label) {
    assert_label(label);
    (REG[rs1] < REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}

void bltu(int rs1, int rs2, int offset) {
    if ((ui) REG[rs1] < (ui) REG[rs2]) {
        if (good_imm(offset)) program_counter = program_counter + 2 * offset;
        else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BLTU. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}

void bge(int rs1, int rs2, string label) {
    assert_label(label);
    (REG[rs1] >= REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}

void bge(int rs1, int rs2, int offset) {
    if (REG[rs1] >= REG[rs2]) {
        if (good_imm(offset)) program_counter = program_counter + 2 * offset;
        else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BGE. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}



void bgeu(int rs1, int rs2, string label) {
    assert_label(label);
    ((ui)REG[rs1] >= (ui)REG[rs2] ? program_counter = labToAdd[label] : program_counter += 4);
}

void bgeu(int rs1, int rs2, int offset) {
    if ((ui) REG[rs1] >= (ui) REG[rs2]) {
        if (good_imm(offset)) program_counter = program_counter + 2 * offset;
        else {
            cout << "ERROR: INPUTTING MORE THAN 12 BITS IN OFFSET - BEGU. TERMINATED!" << endl;
            system("pause");
            exit(1);
        }
    } else program_counter += 4;
}

void jal(int rd, string label) {
    assert_label(label);
    if (!rd) REG[rd] = program_counter + 4;
    program_counter = labToAdd[label];
}
void jal(int rd, int offset) {
    if (rd != 0) REG[rd] = program_counter + 4;
    if (good_imm(offset)) program_counter = program_counter + (offset << 1);
    else {    
        cout << "ERROR: INPUTTING MORE THAN 20 BITS IN OFFSET - JAL. TERMINATED!" << endl;
        quit();
    }
}

void lui(int rd, int imm) {
    if (!rd) return;
    if (good_imm(imm)) REG[rd] = (imm << 12);
    else {
        cout << "ERROR: INPUTTING MORE THAN 20 BITS IN OFFSET - LUI. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}

void auipc(int rd, int imm) {
    if (!rd) return;
    if (good_imm(imm)) REG[rd] = program_counter + (imm << 12);
    else {
        cout << "ERROR: INPUTTING MORE THAN 20 BITS IN OFFSET - AUIP. TERMINATED!" << endl;
        quit();
    }
    program_counter += 4;
}


