#include "cheats.h"

#include <map>
#include <algorithm>

using namespace std;


map< vector<sf::Keyboard::Key>, pair<CheatCallback*,void*>> cheats;
vector<sf::Keyboard::Key> keys;
int maxSize = 0;

void Cheats::clear(){
    cheats.clear();
}

void Cheats::erase(vector<sf::Keyboard::Key> cheat){
    cheats.erase(cheat);
}

void Cheats::add(vector<sf::Keyboard::Key> cheat, CheatCallback* callback, void* data){
    cheats[cheat] = pair<CheatCallback*,void*>(callback,data);
    if(cheat.size()>maxSize)
        keys.resize(maxSize = cheat.size());
}

void addKey(sf::Keyboard::Key key){
    if(keys.size()==0) return;
    for(int i=keys.size()-1; i>=1; i--)
        keys[i] = keys[i-1];
    keys[0] = key;
}

int Cheats::parseEvent(const sf::Event& event){
    int n = 0;
    if(keys.size()==0)
        return n;
    if(event.type == sf::Event::KeyPressed){
        addKey(event.key.code);
        for(const pair< vector<sf::Keyboard::Key>, pair<CheatCallback*,void*>>& cheat:cheats)
            if(search(keys.begin(), keys.end(), cheat.first.rbegin(), cheat.first.rend()) == keys.begin()){
                ++n;
                cheat.second.first(cheat.first,cheat.second.second);
            }
    }
    return n;
}
