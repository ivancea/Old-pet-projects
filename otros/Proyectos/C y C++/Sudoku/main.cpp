#include <iostream>
#include "sudoku.h"

using namespace std;


           ;         //003020600900305001001806400008102900700000008006708200002609500800203009005010300
                     //4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......

int main()
{
    sudoku *s1 = new sudoku(), *s2;
    cout << s1->leer("003020600900305001001806400008102900700000008006708200002609500800203009005010300") << endl;
    cout << *s1 << endl;
    cout << (s2 = resolver(s1)) << endl;
    if(s2!=0) s1=s2;
    cout << *s1 << endl;
}