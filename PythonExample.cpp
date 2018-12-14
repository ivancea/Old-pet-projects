#include <iostream>
#include <Python.h>

using namespace std;

int main(){
    Py_Initialize();
    PyRun_SimpleStringFlags("var = str(2**100)", NULL);
    PyObject *m = PyImport_AddModule("__main__");
    PyObject *v = PyObject_GetAttrString(m,"var");
    string num = PyUnicode_AsUTF8(v);
    cout << num << endl;
}
