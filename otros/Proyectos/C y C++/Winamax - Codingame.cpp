#include <iostream>
#include <queue>

using namespace std;

void readCards(queue<int>& q){
    int n;
    cin >> n;
    for(int i=0; i<n; i++){
        string t;
        cin >> t;
        t.erase(t.size()-1);
        switch(t[0]){
            case 'J': q.push(11); break;
            case 'Q': q.push(12); break;
            case 'K': q.push(13); break;
            case 'A': q.push(14); break;
            default: q.push(stoi(t)); break;
        }
        }
}
    
void push(queue<int>& q, queue<int>& source, int n=-1){
    if(n==-1) n=source.size();
    while(n-- && source.size()){
        q.push(source.front());
        source.pop();
    }
}

int main()
{
    queue<int> a, b;
    
    readCards(a);
    readCards(b);
    
    queue<int> prizeA, prizeB;
    bool pat=false;
    int rounds = 0;
    
    while(a.size() && b.size()){ // A round per cycle
        if(a.front()>b.front()){
            push(a,prizeA);
            push(a,a,1);
            push(a,prizeB);
            push(a,b,1);
        }else if(a.front()<b.front()){
            push(b,prizeA);
            push(b,a,1);
            push(b,prizeB);
            push(b,b,1);
        }else{ // War
            push(prizeA,a,4);
            push(prizeB,b,4);
            if(!a.size() || !b.size())
                pat=true;
            continue;
        }
        ++rounds;
    }
    
    if(pat)
        cout << "PAT" << endl;
    else
        cout << (a.size()==0?"2 ":"1 ") << rounds << endl;
}