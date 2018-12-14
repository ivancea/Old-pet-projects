#include <iostream>
using namespace std;

#define TRAP() __asm__ volatile ("jmp start\n"\
                      "call start\n"\
                      "call start\n"\
                      "jmp start\n"\
                      "nop\n"\
                      "call start\n"\
                      "ret\n"\
                      "ret\n"\
                      "nop\n"\
                      "start:\n"\
                      "call trapping\n"\
                      "ret\n"\
                      "jmp fin\n"\
                      "trapping:\n"\
                      "    incw (%esp)\n"\
                      "    ret\n"\
                      "fin:");

int main(){
    cout << 'a' << endl;
    TRAP();
    cout << 'b' << endl;
}
