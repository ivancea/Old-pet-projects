#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv){
	if(argc==3){
		ifstream a(argv[1]),
				 b(argv[2]);
		int i = 1;
		while(a && b){
			string t1,t2;
			getline(a, t1);
			getline(b, t2);
			if(t1!=t2){
				cout << "Line " << i << ": \"" << t1 << "\" - \"" << t2 << "\"" << endl;
			}
			++i;
		}
	}else cout << "Invalid arguments" << endl;
	cin.get();
}