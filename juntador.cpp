#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

string parseChars(wstring s){
    char w[MAX_PATH];
    int n = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.size(), w, MAX_PATH, NULL, NULL);
    w[n]='\0';

    return w;
}
/*
wstring parseChars2(string s){
    wchar_t w[MAX_PATH];
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size(), w, MAX_PATH);
    w[n]=L'\0';
    return wstring(w);
}
string multiByteToChar(string s){
    char b[MAX_PATH];
    wcstombs(b, parseChars2(s).c_str(), MAX_PATH);
    return string(b);
}

string toAsciiFirst(wstring s){
    return multiByteToChar(parseChars(s));
}*/

string toAscii(wstring s){
    char b[MAX_PATH];
    wcstombs(b, s.c_str(), MAX_PATH);
    return b;
}

void put(size_t v, ofstream& f){
    f.write((char*)&v, sizeof(v));
}

void putString(const wstring& s, ofstream& f){
    put(s.size(), f);
    f.write(parseChars(s).c_str(), s.size());
}

void searcher(bool (*fn)(wstring&, WIN32_FIND_DATAW&, void*), void* val, wstring s=L""){
    if(fn==nullptr)
        return;
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
        /**if(wf.nFileSizeHigh==0 && wf.nFileSizeLow==0)
            searcher(fn, val, s+wf.cFileName);
        else**/{
            if((*fn)(s,wf,val)) break;
        }
    }while(FindNextFileW(h,&wf));
    FindClose(h);
}

bool folderToFileSearcherFunction(wstring& path, WIN32_FIND_DATAW& wf, void* data){
    ofstream &out = *(ofstream*)data;

    putString(wf.cFileName, out);
    char type;
    if(wf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){ /** FOLDER **/
        type = 0;
        out.write(&type, sizeof(type));
        size_t numFiles = 0;
        searcher([](wstring& path, WIN32_FIND_DATAW& wf, void* data)->bool{
                    ++(*((size_t*)data));
                    return false;
                 }, &numFiles, path+wf.cFileName);
        put(numFiles, out);
        searcher(&folderToFileSearcherFunction, data, path+wf.cFileName);
    }else{                                         /** FILE **/
        wcout << path << wf.cFileName << endl;

        type = 1;
        out.write(&type, sizeof(type));
        ifstream inFile(toAscii(path+wf.cFileName), ios::ate|ios::binary);
        put(inFile.tellg(), out);
        inFile.seekg(0);
        out << inFile.rdbuf();
    }
    return false;
}

bool folderToFile(wstring folder, string file){
    ofstream out(file, ios::trunc|ios::binary);
    if(!out)
        return false;
    size_t numFiles = 0;
    searcher([](wstring& path, WIN32_FIND_DATAW& wf, void* data)->bool{
                ++(*((size_t*)data));
                return false;
            }, &numFiles, folder);
    put(numFiles, out);
    searcher(&folderToFileSearcherFunction, (void*)&out, folder);
    out.close();
}

int main(){
    folderToFile(L"canal", "out.dat");
}

/**

[STRING]
4 bytes: Size
X bytes: String

[ELEMENT]
STRING: Name
1 byte: 0/1 -> Folder/File
    [FOLDER]
    4 bytes: Num. elements
    X ELEMENTS
    [FILE]
    STRING: Data

[*BASE*]
4 bytes: Num elements
X ELEMENTS

**/
