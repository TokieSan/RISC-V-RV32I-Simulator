#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef unsigned int ui;

#define MAX ((1ll << 32) - 1)
#define BYTREP 255
#define HALFREP 65535

bool good_imm(int n);
void open_f(string file_tmp);
string clean(string str, bool lowercase);
void quit();
void get_file();
void output_hex();
void output_dec();
void output_bin();
vector<string> csv_to_vec(string s);
void init_regToId();
void process_file();
void execute(string s);
vector<string> with_par(string instruction_more);
void init();
void run_screen();

#include "instructions.h"

int main() {
    init();
    run_screen();
    return 0;
}

bool good_imm(int n) {
    return (!(n > (1 << 11) - 1 || n < -1 * (1 << 11)));
}
void open_f(string file_tmp) {
    curFile.open(file_tmp);
    while (!curFile.is_open()) cin >> file_tmp, curFile.open(file_tmp);
}
string clean(string str, bool lowercase) {
    while(str.size() && ((*str.begin()) == ' ' || (*str.begin()) == '\n') || ((*str.begin()) == '\t')) str.erase(str.begin());
    while(str.size() && (str.back() == ' ' || str.back() == '\n' || str.back() == '\t')) str.pop_back();
    if(lowercase) for(auto&u : str) if(('A' <= u && u <= 'Z')) u = tolower(u);
    return str;
}
void quit() {
    exit(1);
}
void get_file() {
    string file_tmp;
    cin >> file_tmp;
    open_f(file_tmp);
}

void output_hex() {
    cout << "OUTPUT IN HEX:\n";
    if(MEM.size()) for(auto&u : MEM) cout << setw(8) << setfill('0') << uppercase << hex << u.first << " : " << u.second << '\n';
    cout << "REG.secondTE.second:\n";
    for(int i = 0; i < 32; i++) cout << setw(8) << "x" + to_string(i) << " : "  << setfill('0') << uppercase << hex << REG[i] << '\n';
}
void output_dec() {
    cout << "OUTPUT IN DECIMAL:\n";
    if(MEM.size()) for(auto&u : MEM) cout << setw(8) << u.first << " : " << u.second << '\n';
    cout << "REG.secondTE.second:\n";
    for(int i = 0; i < 32; i++) cout << "x" << i << " : " << REG[i] << '\n';
}
void output_bin() {
    cout << "OUTPUT IN BINARY:\n";
    if(MEM.size())
        for(auto&u : MEM) {
            to_bin = u.second;
            cout << setw(12) << u.first << " : " << to_bin << '\n';
        }
    cout << "REG.secondTE.second:\n";
    for(int i = 0; i < 32; i++) to_bin = REG[i], cout << "x" << i << " : " << to_bin << '\n';
}

vector<string> csv_to_vec(string s) {
    string cur = "";
    vector<string> v;
    stringstream ss(s);
    for(auto&u : s) {
        if(u == ',') {
            cur = clean(cur, 0);
            v.push_back(cur);
            cur = "";
        } else cur += u;
    }
    if(cur.size()) v.push_back(clean(cur, 0));
    return v;
}

void init_regToId() {
    for(int i = 0; i <= 31; i++) regToId["x" + to_string(i)] = i;
    regToId["zero"] = 0;
    regToId["ra"] = 1;
    regToId["sp"] = 2;
    regToId["gp"] = 3;
    regToId["tp"] = 4;
    regToId["t0"] = 5;
    regToId["t1"] = 6;
    regToId["t2"] = 7;
    regToId["s0"] = 8;
    regToId["s1"] = 9;
    regToId["a0"] =  10;
    regToId["a1"] =  11;
    regToId["a2"] =  12;
    regToId["a3"] =  13;
    regToId["a4"] =  14;
    regToId["a5"] =  15;
    regToId["a6"] =  16;
    regToId["a7"] =  17;
    regToId["s2"] =  18;
    regToId["s3"] =  19;
    regToId["s4"] =  20;
    regToId["s5"] =  21;
    regToId["s6"] =  22;
    regToId["s7"] =  23;
    regToId["s8"] =  24;
    regToId["s9"] =  25;
    regToId["s10"] = 26;
    regToId["s11"] = 27;
    regToId["t3"] =  28;
    regToId["t4"] =  29;
    regToId["t5"] =  30;
    regToId["t6"] =  31;
}
void process_file() {
    int cur_pc = program_counter;
    string s;
    while(getline(curFile, s)) {
        if(!s.size()) break;
        s = clean(s, 0);
        bool fg = 0;
        for(auto&u : s) if(u == ':') {
                fg = 1;
                break;
            }
        if(!fg) logs[cur_pc] = s, cur_pc += 1 << 2;
        else {
            string label = "";
            string initialize = "";
            bool fg2 = 0;
            for(auto&u : s) {
                if(u == ':') {
                    fg2 = 1;
                    continue;
                }
                fg2 ? initialize += u : label += u;
            }
            label = clean(label, 0), initialize = clean(initialize, 0);
            if(labToAdd.count(label)) {
                cout << "Duplicate Label\n";
                quit();
            }
            if(label[0] >= '0' && label[0] <= '9') {
                cout << "firstirst character of label is digit, forbidden\n";
                quit();
            }
            labToAdd[label] = cur_pc;
            if(initialize.size()) logs[cur_pc] = initialize, cur_pc += 1 << 2;
        }
    }
}
void execute(string s) {
    stringstream ss(s);
    string inst;
    ss >> inst;
    inst = clean(inst, 1);
    string instruction_more;
    getline(ss, instruction_more);
    instruction_more = clean(instruction_more, 0);
    vector<string> IS = csv_to_vec(instruction_more);
    for(auto&u : IS) u = clean(u, 0);
    if(find(IS[1].begin(), IS[1].end(), '(') != IS[1].end()) IS = with_par(instruction_more);
    if(inst == "add") add(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "and") _and(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "sub")sub(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "or") _or(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "xor") _xor(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "srl") {
        IS = csv_to_vec(instruction_more), srl(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
        for(auto&u : IS) u = clean(u, 0);
    }
    if(inst == "sra") sra(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "sll") sll(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "slt") slt(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "sltu") sltu(regToId[IS[0]], regToId[IS[1]], regToId[IS[2]]);
    if(inst == "andi") andi(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "ori") ori(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "xori") xori(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "addi") addi(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "slli") slli(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "srai") srai(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "slti") slti(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "sltiu") sltiu(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "srli") srli(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lw") lw(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lh") lh(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lb") lb(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lhu") lhu(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "jalr") jalr(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "sw") sw(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "sh") sh(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "sb") sb(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lbu") lbu(regToId[IS[0]], regToId[IS[1]], stoi(IS[2]));
    if(inst == "lui") lui(regToId[IS[0]], stoi(IS[1]));
    if(inst == "jal" && isdigit(IS[1][0]))  jal(regToId[IS[0]], stoi(IS[1]));
    else if(inst == "jal")  jal(regToId[IS[0]], IS[1]);
    if(inst == "auipc") auipc(regToId[IS[0]], stoi(IS[1]));
    if(inst == "beq") isdigit(IS[2][0]) ? beq(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : beq(regToId[IS[0]], regToId[IS[1]], IS[2]);
    if(inst == "blt") isdigit(IS[2][0]) ? blt(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : blt(regToId[IS[0]], regToId[IS[1]], IS[2]);
    if(inst == "bltu") isdigit(IS[2][0]) ? bltu(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : bltu(regToId[IS[0]], regToId[IS[1]], IS[2]);
    if(inst == "bne") isdigit(IS[2][0]) ? bne(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : bne(regToId[IS[0]], regToId[IS[1]], IS[2]);
    if(inst == "bge") isdigit(IS[2][0]) ? bge(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : bge(regToId[IS[0]], regToId[IS[1]], IS[2]);
    if(inst == "bgeu") isdigit(IS[2][0]) ? bgeu(regToId[IS[0]], regToId[IS[1]], stoi(IS[2])) : bgeu(regToId[IS[0]], regToId[IS[1]], IS[2]);
}
vector<string> with_par(string instruction_more) {
    vector<string> IS_DET = csv_to_vec(instruction_more);
    string immediate = "", sec = "";
    bool fg = 0;
    for(auto&u : IS_DET[1]) {
        if(u == ')') break;
        if(u == '(') {
            fg = 1;
            continue;
        }
        fg ? sec += u : immediate += u;
    }
    immediate = clean(immediate, 0);
    if(!immediate.size()) immediate += '0';
    sec = clean(sec, 0);
    assert(sec.back() != ' ');
    assert(immediate.back() != ' ');
    IS_DET.pop_back(), IS_DET.push_back(sec), IS_DET.push_back(immediate);
    return IS_DET;
}
void init() {
    init_regToId();
}
void run_screen() {
    cout << "WELCOME\n";
    string t;
    cout << "Please write initial address:\n";
    cin >> program_counter;
    memset(REG, 0, sizeof REG);
    cout << "Write yes if you want to intialize memory\n";
    cin >> t;
    if(t == "yes") {
        cout << "Please enter the memory file name\n";
        get_file();
        ll val, address;
        while (curFile >> address) {
            curFile >> val;
            MEM[address] = val;
        }
        curFile.close();
    }
    cout << "Please enter assembly code file name\n";
    get_file();
    process_file();
    cout << "Please enter the base you want to print in, dec or bin or hex\n";
    string base;
    cin >> base;
    while (1) {
        string u = logs[program_counter].substr(0, 5);
        if(clean(u, 1) == "fence" || clean(u, 1) == "ecall" || clean(u, 1) == "ebrea") break;

        cout << "PC : " << dec << program_counter << "\n ISRUCTION: " << logs[program_counter] << '\n';
        execute(logs[program_counter]);
        cout << setw(8);
        if(base == "hex") output_hex();
        else if(base == "bin") output_bin();
        else output_dec();
    }

}


