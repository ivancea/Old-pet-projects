#include <iostream>
#include <fstream>

using namespace std;

bool executeBrainfuck(const string& code, string& memory){
	unsigned int instructionPointer = 0;
	unsigned int memoryPointer = 0;
	
	if(memory.size() == 0){
		cout << "The memory must have at least a size of 1" << endl;
		return false;
	}
	
	{ // Check loops
		int depth = 0;
		for(int i = 0; i < code.size(); i++){
			if(code[i] == '['){
				++depth;
			}else if(code[i] == ']'){
				--depth;
				
				if(depth < 0){
					cout << "Invalid loop close at " << i << endl;
					return false;
				}
			}
		}
	}
	
	while(instructionPointer < code.size()){
		switch(code[instructionPointer]){ // "<>[]+-., \t\n\r"
			case '<':
				if(memoryPointer == 0){
					cout << "\nEXECUTION ERROR: Memory pointer below 0" << endl;
					return false;
				}
				--memoryPointer;
				break;
				
			case '>':
				if(memoryPointer == memory.size()-1){
					cout << "\nEXECUTION ERROR: Memory pointer below 0" << endl;
					return false;
				}
				++memoryPointer;
				break;
				
			case '+':
				++memory[memoryPointer];
				break;
				
			case '-':
				--memory[memoryPointer];
				break;
				
			case '.':
				cout << memory[memoryPointer];
				break;
				
			case ',':
				memory[memoryPointer] = cin.get();
				break;
				
			case '[':
				if(memory[memoryPointer] == 0){
					int depth = 1;
					do{
						++instructionPointer;
						if(code[instructionPointer] == '['){
							++depth;
						}else if(code[instructionPointer] == ']'){
							--depth;
						}
					}while(depth > 0);
				}
				break;
				
			case ']':
				if(memory[memoryPointer] != 0){
					int depth = 1;
					do{
						--instructionPointer;
						if(code[instructionPointer] == '['){
							--depth;
						}else if(code[instructionPointer] == ']'){
							++depth;
						}
					}while(depth > 0);
				}
				break;
			
			default:
				break;
		}
		++instructionPointer;
	}
	
	return true;
}

string executeBrainfuck(const string& code, int memorySize){
	string memory(memorySize, 0);
	executeBrainfuck(code, memory);
	return memory;
}

int main(int argc, char** argv){
	if(argc != 2){
		cout << "Pass the file with the code as the first argument" << endl;
		return 1;
	}
	
	ifstream codeFile(argv[1]);
	
	if(!codeFile){
		cout << "Couldn't read the file" << endl;
		return 1;
	}
	
	string code;
	while(codeFile){
		code += codeFile.get();
	}
	
	executeBrainfuck(code, 100000);
	
	cout << "\n\nExecution ended" << endl;
	cin.get();
	
	return 0;
}
