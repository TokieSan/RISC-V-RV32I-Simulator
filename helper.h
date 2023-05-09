#pragma once

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

bool good_imm(int n);
void open_f(string file_tmp);
string clean(string str, bool lowercase);
void quit();
void get_file();
void output_hex();
void output_dec();
void output_bin();
V<string> csv_to_vec(string s);
void init_regToId();
void process_file();
void execute(string s);
V<string> with_par(string instruction_more);
void init();
void run_screen();
