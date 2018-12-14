#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

int find(const string& str, string chars, int pos){
	int min = string::npos;
	for(char c:chars){
		int n = str.find(c, pos);
		if(min == string::npos || (n != string::npos && n<min))
			min = n;
	}
	return min;
}

vector<string> split(const string& txt){
	vector<string> v;
	int p1 = 0, p2=-1;
	do{
		p1 = p2+1;
		p2 = find(txt, ",._'\" \n\r", p1+1);
		if(p2!=string::npos){
			if(p2-p1>0)
				v.push_back(txt.substr(p1, p2-p1));
			v.push_back(txt.substr(p2,1));
		}else
			v.push_back(txt.substr(p1));
	}while(p2 != string::npos);
	return v;
}

void convert(string& txt){
	auto rnd = std::default_random_engine(time(0));
	vector<string> words = split(txt);
	txt.clear();
	for(string& str:words){
		if(str.size()<4){
			txt += str;
			continue;
		}
		char initialChar = str.front(), finalChar = str.back();
		string toShuffle = str.substr(1, str.size()-2);
		shuffle(toShuffle.begin(), toShuffle.end(), rnd);
		txt += initialChar + toShuffle + finalChar;
	}
}

void convert2(string& str){
	auto rnd = std::default_random_engine(time(0));
	auto it1 = str.begin(), it2 = str.begin();
	int count = 0;
	string chars = "¡!¿?[]{}()/&%$^·,.+*-_ '\"\n\r";
	while(it2 != str.end()){
		if(chars.find(*it2) != string::npos){
			if(count>3){
				auto temp1 = it1, temp2 = it2;
				temp1++;
				temp2--;
				shuffle(temp1, temp2, rnd);
			}
			count = 0;
			it1 = it2;
		}else{
			if(count == 0)
				it1 = it2;
			++count;
		}
		it2++;
	}
}

int main(){
	string txt;
	fstream f("input.txt", ios::in);
	if(!f)
		return 1;
	while(!f.eof()){
		string t;
		getline(f,t);
		cout << t << endl;
		txt += t;
		if(f.eof())
			txt += "\n";
	}
	cout << "Readed" << endl;
	convert2(txt);
	cout << txt << endl;
	cout << "Converted" << endl;
	f.close();
	f.open("output.txt", ios::out | ios::trunc);
	f << txt;
	f.close();
}