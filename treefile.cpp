#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

void replaceAll(string& str, string findStr, string replacedStr){
	size_t p;
	do{
		p = str.find(findStr, p+replacedStr.size());
		str.replace(p,findStr.size(), replacedStr);
	}while(p!=string::npos);
}

string toUTF(wstring s){
    char w[MAX_PATH];
    int n = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.size(), w, MAX_PATH, NULL, NULL);
    w[n]='\0';
    return w;
}

void searcher(bool (*fn)(wstring&, WIN32_FIND_DATAW&, void*), void* val, wstring s=L""){
    if(s.size()!=0 && s[s.size()-1]!=L'/')
        s+=L'/';
    if(s[s.size()-1]!=L'*')
        s+=L'*';
    WIN32_FIND_DATAW wf;
    HANDLE h = FindFirstFileW(s.c_str(),&wf);
    if(h==INVALID_HANDLE_VALUE) return;
		s.erase(s.size()-1);
    do{
        if(!wcscmp(wf.cFileName,L"..") || !wcscmp(wf.cFileName,L".")) continue;
        if(wf.nFileSizeHigh==0 && wf.nFileSizeLow==0)
            searcher(fn, val, s+wf.cFileName);
        else{
            if((*fn)(s,wf,val)) break;
        }
    }while(FindNextFileW(h,&wf));
    FindClose(h);
}

void treeFiles(bool (*fn)(wstring& url,WIN32_FIND_DATAW& fileInfo, void* val)=0, void* val=0){
    if(fn==0)
        searcher([](wstring& url,WIN32_FIND_DATAW& wf, void* val)->bool{return false;},val);
    else
        searcher(fn,val);
}

bool func(wstring& url,WIN32_FIND_DATAW& wf, void* val){
    /** Finder **/
    /*if(contains(toAscii(wf.cFileName),*(string*)val))
        wcout << url << wf.cFileName << endl;*/
    ifstream f(toUTF(url+wf.cFileName));
    string t;
    while(f){
        char a[1024];
        f.read(a, 1024);
        t += string(a, f.gcount());
    }
    f.close();
    if(t.find(*(string*)val) != string::npos){
        wcout << url << wf.cFileName << endl;
        /**replaceAll(t, *(string*)val, "ZoneSaver");
        ofstream fout(toAscii(url+wf.cFileName), ios::trunc);
        fout.write(t.c_str(), t.size());**/
    }

    /** Último acceso **/
    /*SYSTEMTIME st;
    FileTimeToSystemTime(&wf.ftLastAccessTime, &st);
    if(2014>st.wYear || 8>st.wMonth || 1>st.wDay)
        return;
    size_t size = wf.nFileSizeHigh*(MAXDWORD+1) + wf.nFileSizeLow;
    for(int i=0; i<80; i++)
        wcout << L'*';
    wcout << endl << wf.cFileName << endl;
    wcout << L'\t' << "Size: " << size << " bytes" << endl;
    wcout << L'\t' << L"Last accesed: " << st.wDay << L'/' << st.wMonth << L'/' << st.wYear << endl << endl;*/

    return false;
}

struct LinesCounter{
	vector<string> extensions;
	uint64_t lines;
	
	LinesCounter():lines(0){}
};

bool linesFunc(wstring& url,WIN32_FIND_DATAW& wf, void* val){
	LinesCounter* lc = (LinesCounter*)val;
    string name = toUTF(wf.cFileName);
	size_t p = name.rfind('.');
	if(p==string::npos){
		return false;
	}
	name = name.substr(p+1);
	
	bool found = false;
	for(const string& ext : lc->extensions)
		if(name==ext){
			found = true;
			break;
		}
    if(found){
        ifstream f(toUTF(url+wf.cFileName));
        while(f){
            ++lc->lines;
            string t;
            getline(f, t);
        }
    }
    return false;
}

bool replaceFunc(wstring& url, WIN32_FIND_DATAW& wf, void* val){
    string path = toUTF(url+wf.cFileName);
    ifstream f(path);
    string t;
    while(f){
        char a[1024];
        f.read(a, 1024);
        t += string(a, f.gcount());
    }
    f.close();
    if(t.find(*(string*)val) != string::npos){
        wcout << url << wf.cFileName << endl;
        replaceAll(t, *(string*)val, ((string*)val)[1]);
        ofstream fout(path, ios::trunc);
        fout.write(t.c_str(), t.size());
    }
    return false;
}

bool findFunc(wstring& url,WIN32_FIND_DATAW& wf, void* val){
    if(toUTF(wf.cFileName).find(*(string*)val) != string::npos){
        wcout << url << wf.cFileName << endl;
        return false;
    }
    uint64_t size = wf.nFileSizeHigh*((uint64_t)MAXDWORD+1) + wf.nFileSizeLow;
	if(size<1024*1024*256){
		ifstream f(toUTF(url+wf.cFileName));
		string t;
		while(f){
			char a[1024];
			f.read(a, 1024);
			t += string(a, f.gcount());
		}
		f.close();
		if(t.find(*(string*)val) != string::npos)
			wcout << url << wf.cFileName << endl;
	}
    return false;
}

bool sizeFunc(wstring& url,WIN32_FIND_DATAW& wf, void* val){
    uint64_t *size = (uint64_t*)val;
    uint64_t actual = wf.nFileSizeHigh*((uint64_t)MAXDWORD+1) + wf.nFileSizeLow;
    (*size) += actual;
    return false;
}

int main(){
    while(true){
        string t;
        wcout << "Options:\n\n0) Exit\n1) Find a string\n2) Replace a string\n3) Find the folder size\n4) Count files lines\n\n>> " << flush;
        getline(cin,t);
        if(t=="1"){
            string txt;
            wcout << "Texto a buscar: " << flush;
            getline(cin,txt);
            treeFiles(&findFunc, (void*)&txt);
        }else if(t=="2"){
            string txt[2];
            wcout << "Texto a buscar: " << flush;
            getline(cin,txt[0]);
            wcout << "Reemplazar por: " << flush;
            getline(cin,txt[1]);
            treeFiles(&replaceFunc, (void*)txt);
        }else if(t=="3"){
            uint64_t size = 0;
            treeFiles(&sizeFunc, (void*)&size);
            wcout << endl << size/1024 << " KB" << endl;
        }else if(t=="4"){
			LinesCounter lc;
			string ext;
			wcout << "Write the file extensions to count lines (nothing to finish):" << endl;
			do{
				wcout << ">> " << flush;
				getline(cin, ext);
				if(ext.size()!=0)
					lc.extensions.push_back(ext);
			}while(ext.size()>0);
			if(lc.extensions.size()>0)
				treeFiles(&linesFunc, (void*)&lc);
            wcout << endl << lc.lines << " lines." << endl;
        }else if(t=="0"){
			break;
		}else{
			wcout << "Incorrect option..." << endl;
			continue;
		}
		wcout << "Finished." << endl << endl;
    }
}
