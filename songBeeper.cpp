#include <windows.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <list>

using namespace std;


std::string trim(std::string s, char c=' '){
    int i=0;
    while(i<s.size() && s[i]==c)
        ++i;
    s.erase(0,i);
    i = s.size()-1;
    while(i>=0 && s[i]==c)
        --i;
    s.erase(i+1,s.size());
    return s;
}

struct Element{
	int note;
	int octave;
	float duration;
};

enum Notas{
    DO=1,
    DO_S=2,
    RE,
    RE_S,
    MI,
    FA,
    FA_S,
    SOL,
    SOL_S,
    LA,
    LA_S,
    SI
};

bool getNote(string str, Element& elem){
	// NOTE-OCTAVE-[b/#]
	
	static const map<string, int> notes {
		{"DO", 1},
		{"RE", 3},
		{"MI", 5},
		{"FA", 6},
		{"SOL", 8},
		{"LA", 10},
		{"SI", 12}
	};
	
	if(str.size() < 3)
		return false;
	
	char offset = 0;
	if(str.back() == '#')
		offset = 1;
	else if(str.back() == 'b')
		offset = -1;
	
	size_t pos = str.find_first_of("0123456789-");
	if(pos == string::npos)
		return false;
	
	string note = str.substr(0, pos);
	
	auto it = notes.find(note);
	if(it == notes.end())
		return false;
	elem.note = it->second + offset;
	
	str = str.substr(pos);
	try{
		elem.octave = stoi(str);
	}catch(...){
		return false;
	}
	
	if(elem.note == 0){
		elem.note = 12;
		elem.octave -= 1;
	}else if(elem.note == 13){
		elem.note = 1;
		elem.octave += 1;
	}
	
	return true;
}

int main(int argc, char** argv){
	if(argc != 2){
		cout << "Drop a song on this executable..." << endl;
		cout << "The file format is:" << endl;
		cout << "First line: <N>" << endl;
		cout << " - N: the duration, in milliseconds, of a black" << endl;
		cout << "Other lines, can be a NOTE or a REST:" << endl;
		cout << "NOTE: <NOTE><O>[b/#] <duration>" << endl;
		cout << "SILENCE: duration" << endl;
		cout << " - NOTE: DO, RE, MI, FA, SOL, LA, SI" << endl;
		cout << " - O: the octave of the note" << endl;
		cout << " - b/# (optional): b for flat, # for sharp" << endl;
		cout << " - duration: the duration of the note, in quarters (greater than zero, with decimals)" << endl;
		cin.get();
		return 0;
	}
	
	ifstream file(argv[1]);
	if(!file){
		cout << "Cannot open the file..." << endl;
		cin.get();
		return 0;
	}
	
	int line = 1;
	
	string t;
	getline(file, t);
	int ms = atoi(t.c_str());
	if(ms <= 0){
		cout << "Invalid ms (must be a value over 0)..." << endl;
		cin.get();
		return 0;
	}
	
	list<Element> elements;
	
	while(file){
		++line;
		getline(file, t);
		t = trim(t);
		if(t.size() == 0)
			continue;
		
		size_t pos = t.find(' ');
		
		Element element;
		if(pos != string::npos){
			if(!getNote(t.substr(0, pos), element)){
				cout << "Error at line " << line << "..." << endl;
				cin.get();
				return 0;
			}
			t = t.substr(pos+1);
			element.duration = atof(t.c_str());
			if(element.duration <= 0){
				cout << "Error at line " << line << "..." << endl;
				cin.get();
				return 0;
			}
			elements.push_back(element);
		}else{
			element.duration = atof(t.c_str());
			if(element.duration <= 0){
				cout << "Error at line " << line << "..." << endl;
				cin.get();
				return 0;
			}
			element.note = -1;
			elements.push_back(element);
		}
	}
	
	for(const Element& elem : elements){
		if(elem.note < 0){
			Sleep(elem.duration * ms);
		}else{
			cout << elem.note << " " << elem.octave << " - " << elem.duration << endl;
			double n = 440.0 * exp((((double)elem.octave-4)+((double)elem.note-10)/12)*log(2));
			if(n>37 && n<32767){
				Beep(n, elem.duration * ms);
			}
		}
	}
}