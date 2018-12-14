#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(){
    vector< vector<int> > v;
    ifstream f("triangle.txt");
    int temp=0;
    for(int i=0; !f.eof(); i++){
        v.push_back(vector<int>());
        for(int j=0; j<=i; j++){
            f >> temp;
            v[i].push_back(temp);
        }
    }
    while(v.size()>1){
        for(int i=0; i<v[v.size()-2].size();i++)
            v[v.size()-2][i] = v[v.size()-2][i] + (v[v.size()-1][i] > v[v.size()-1][i+1]?v[v.size()-1][i]:v[v.size()-1][i+1]);
        v.erase(v.end());
    }
    cout << v[0][0];
    return 0;
}
