#include <iostream>
#include <fstream>
#include <list>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "Status.h"

using namespace std;


struct TimeLapseStatus{
    int status;

    time_t start;
    time_t end;
};

bool parseFile(ifstream& inFile, list<TimeLapseStatus>& statusList, bool falsifyErrors = true){
    string line;
    TimeLapseStatus actualLapse;
    bool mustInitializeActualLapse = true;

    while(inFile){
        getline(inFile, line);
        if(line.size() == 0)
            continue;
        size_t pos = line.find(':');
        if(pos == string::npos)
            return false;

        time_t itemTime = stoi(line.substr(0, pos));
        int itemStatus = stoi(line.substr(pos+1));

        if(!mustInitializeActualLapse && itemTime <= actualLapse.end)
            continue;

        if(mustInitializeActualLapse){
            mustInitializeActualLapse = false;
            actualLapse.status = itemStatus;
            actualLapse.start = itemTime;
            actualLapse.end = itemTime;
        }else if(actualLapse.status == itemStatus && itemTime - actualLapse.end < 60){ // 1 min
            actualLapse.end = itemTime;
        }else{
            statusList.push_back(actualLapse);
            actualLapse.status = itemStatus;
            actualLapse.start = (itemTime - actualLapse.end < 60? actualLapse.end : itemTime);
            actualLapse.end = itemTime;
        }
    }
    if(!mustInitializeActualLapse)
        statusList.push_back(actualLapse);

    if(falsifyErrors){
        for(auto it = statusList.begin(); it != statusList.end();){
            auto it2 = it;
            it++;
            if(it != statusList.end()){
                if(it2->status == -1 && it2->end - it2->start <= 300 && it->start - it2->end < 60){ // 5 min or less
                    it->start = it2->start;
                    statusList.erase(it2);
                }
            }
        }
        for(auto it = statusList.begin(); it != statusList.end();){
            auto it2 = it;
            it++;
            if(it != statusList.end()){
                if(it->status == it2->status && it->start - it2->end < 60){ // 1 min
                    it->start = it2->start;
                    statusList.erase(it2);
                }
            }
        }
    }

    return true;
}

bool operator<(const tm& tm1, const tm& tm2){
    if(tm1.tm_year < tm2.tm_year)
        return true;
    if(tm1.tm_year > tm2.tm_year)
        return false;

    return tm1.tm_yday < tm2.tm_yday;
}

map<tm, list<TimeLapseStatus> > divideByDay(list<TimeLapseStatus>& statusList){
    map<tm, list<TimeLapseStatus> > statusByDay;

    /*for(auto it = statusList.begin(); it != statusList.end();){
        tm start = tm(*localtime(&it->start));
        tm temp = tm(*localtime(&it->end));
        if(start.tm_yday != temp.tm_yday){
            temp.tm_hour = temp.tm_min = temp.tm_sec = 0;
            time_t newTime = mktime(&temp);

            TimeLapseStatus newDay;
            newDay.status = it->status;
            newDay.start = it->start;
            newDay.end = newTime-1;

            it->start = newTime;
            it = statusList.insert(it, newDay);
        } else it++;
    }*/

    for(auto it = statusList.begin(); it != statusList.end();){
        tm start = tm(*localtime(&it->start));
        tm temp = tm(*localtime(&it->end));

        if(start.tm_yday != temp.tm_yday){
            start.tm_hour = 23;
            start.tm_min = 59;
            start.tm_sec = 59;

            time_t newTime = mktime(&start);

            TimeLapseStatus newDay;
            newDay.status = it->status;
            newDay.start = it->start;
            newDay.end = newTime;

            it->start = newTime+1;
            it = statusList.insert(it, newDay);
        } else it++;
    }

    for(auto it : statusList){
        tm start = tm(*localtime(&it.start));
        statusByDay[start].push_back(it);
    }

    /*for(auto it : statusList){
        tm start = tm(*localtime(&it.start));
        tm temp = tm(*localtime(&it.end));
        if(start.tm_yday != temp.tm_yday){
            temp = start;
            temp.tm_mday += 1;
            temp.tm_hour = temp.tm_min = temp.tm_sec = 0;
            time_t newTime = mktime(&temp);

            TimeLapseStatus dayA, dayB;
            dayA.status = dayB.status = it.status;
            dayA.start = it.start;
            dayA.end = newTime;
            dayB.end = it.end;
            dayB.start = newTime;
            statusByDay[start].push_back(dayA);
            statusByDay[temp].push_back(dayB);
        }else{
            statusByDay[start].push_back(it);
        }
    }*/

    return statusByDay;
}

void renderGraphic(sf::RenderWindow& window, map<tm, list<TimeLapseStatus> >& statusByDay, int offsetX, int offsetY){
    window.clear(sf::Color::White);

    int index = 0;

    sf::Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(10);
    text.setColor(sf::Color::Black);

    sf::RectangleShape line(sf::Vector2f(1, statusByDay.size()*40));
    for(int i=0; i<=24; i++){
        /*text.setString((i<10?"0":"") + to_string(i) + ":00");
        text.setPosition(80 + 60*2*i - offsetX, -offsetY);
        window.draw(text);*/

        line.setFillColor(sf::Color::Black);
        line.setPosition(80 + 60*2*i - offsetX, 30 - offsetY);
        window.draw(line);
    }

    line.setSize(sf::Vector2f(60*2*24, 1));

    for(auto statusIterator = statusByDay.rbegin(); statusIterator != statusByDay.rend(); statusIterator++){
        pair<const tm, list<TimeLapseStatus> >& statusListPair = *statusIterator;

        for(int i=0; i<24; i++){
            text.setString((i<10?"0":"") + to_string(i) + ":00");
            text.setPosition(100 + 60*2*i - offsetX, 25+index*40-offsetY);
            window.draw(text);
        }

        line.setPosition(80 - offsetX, 50 + index*40 - offsetY);
        window.draw(line);

        const tm& statusDay = statusListPair.first;
        list<TimeLapseStatus>& statusList = statusListPair.second;

        text.setString((statusDay.tm_mday<10?"0":"") + to_string(statusDay.tm_mday) + "/"
                     + (statusDay.tm_mon<10?"0":"") + to_string(statusDay.tm_mon+1) + "/"
                     + to_string(1900 + statusDay.tm_year));
        text.setPosition(5 - offsetX, 40 + index*40 - offsetY);
        window.draw(text);

        for(TimeLapseStatus& status : statusList){
            tm* timeInfo = localtime(&status.start);
            int start = (timeInfo->tm_sec + 60*(timeInfo->tm_min + 60*timeInfo->tm_hour))/30;
            timeInfo = localtime(&status.end);
            int end = (timeInfo->tm_sec + 60*(timeInfo->tm_min + 60*timeInfo->tm_hour))/30;

            sf::RectangleShape rect(sf::Vector2f(end-start, 20));
            rect.setPosition(80 + start - offsetX,
                             40 + index*40 - offsetY);
            switch(status.status % 10){
            case Status::Offline:
                rect.setFillColor(sf::Color(230,230,230));
                break;
            case Status::Online:
                rect.setFillColor(sf::Color::Green);
                break;
            case Status::Busy:
                rect.setFillColor(sf::Color::Red);
                break;
            case Status::Away:
                rect.setFillColor(sf::Color(128,128,255));
                break;
            case Status::Snooze:
                rect.setFillColor(sf::Color(0,0,255));
                break;
            case Status::LookingToTrade:
                rect.setFillColor(sf::Color(128,255,128));
                break;
            case Status::LookingToPlay:
                rect.setFillColor(sf::Color(50,255,50));
                break;
            case Status::Error:
            default:
                rect.setFillColor(sf::Color::Black);
                break;
            }
            if(status.status>=10){
                rect.setFillColor(sf::Color(0,128,0));
            }
            window.draw(rect);
        }

        index++;
    }
}

void startGraphicalInterface(map<tm, list<TimeLapseStatus> >& statusByDay){
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "SteamStalker Graphical View");


    int offsetX = 0,
        offsetY = 0;

    map<sf::Keyboard::Key, bool> keys;
    sf::Vector2i mouse;
    bool mousePressed = false;

    while(window.isOpen()){

        sf::Event ev;
        while(window.pollEvent(ev)){
            switch(ev.type){
            case sf::Event::KeyPressed:
                keys[ev.key.code] = true;
                if(ev.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            case sf::Event::KeyReleased:
                keys[ev.key.code] = false;
                break;

            case sf::Event::MouseMoved:
                if(mousePressed){
                    offsetX += mouse.x - ev.mouseMove.x;
                    offsetY += mouse.y - ev.mouseMove.y;
                    if(offsetX < 0)
                        offsetX = 0;
                    if(offsetY < 0)
                        offsetY = 0;
                }
                mouse = sf::Vector2i(ev.mouseMove.x, ev.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                if(ev.mouseButton.button == sf::Mouse::Left){
                    mousePressed = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if(ev.mouseButton.button == sf::Mouse::Left){
                    mousePressed = false;
                }
                break;

            case sf::Event::Closed:
                window.close();
            }
        }

        renderGraphic(window, statusByDay, offsetX, offsetY);

        if(keys[sf::Keyboard::Up]){
            offsetY -= 5;
            if(offsetY < 0)
                offsetY = 0;
        }else if(keys[sf::Keyboard::Down]){
            offsetY += 5;
        }
        if(keys[sf::Keyboard::Right]){
            offsetX += 5;
        }else if(keys[sf::Keyboard::Left]){
            offsetX -= 5;
            if(offsetX < 0)
                offsetX = 0;
        }

        window.display();
        sf::sleep(sf::milliseconds(10));
    }
}

int main(int argc, char** argv){
    if(argc != 2){
        cout << "Send the log filename as first parameter." << endl;
    }else{
        ifstream inFile(argv[1]);
        if(!inFile){
            cout << "Could not open the file." << endl;
        }else{
            list<TimeLapseStatus> statusList;
            if(!parseFile(inFile, statusList)){
                cout << "Invalid file format: Could not parse data." << endl;
            }else{
                map<tm, list<TimeLapseStatus> > statusByDay = divideByDay(statusList);
                startGraphicalInterface(statusByDay);
            }
        }
    }
    cin.get();
}
