#include <iostream>
#include <fstream>

using namespace std;

int main(){
    const int NUM = 80;
    uint32_t m[NUM][NUM];

    { /// LOAD MATRIX
        ifstream file("matrix.txt");
        for(uint32_t i=0; i<NUM; i++){
            string t;
            getline(file,t);
            for(uint32_t j=0; j<NUM; j++){
                size_t pos = t.find(',');
                m[i][j] = atoi(t.substr(0,pos).c_str());
                t.erase(0,pos+1);
            }
        }
    }

    for(int d = NUM-1; d>=0; d--){ /// DIAGONAL
        for(int i=d; i>=0; i--)
            if(i==NUM-1)
                continue;
            else if(d==NUM-1)
                m[i][d] += m[i+1][d];
            else if(m[i+1][d]<m[i][d+1])
                m[i][d] += m[i+1][d];
            else
                m[i][d] += m[i][d+1];

        if(d==0) break;

        for(int j=d-1; j>=0; j--)
            if(j==NUM-1)
                continue;
            else if(d==NUM-1)
                m[d][j] += m[d][j+1];
            else if(m[d][j+1]<m[d+1][j])
                m[d][j] += m[d][j+1];
            else
                m[d][j] += m[d+1][j];
    }
    cout << m[0][0];
}
