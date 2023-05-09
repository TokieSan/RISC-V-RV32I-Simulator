#include <iostream>

using namespace std;

#include "simulator_main.hxx"

int main() {
    Simulator sim;
    sim.init();
    sim.run_screen();
    return 0;
}
