#include "sceneGame.h"

#include <string>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "cheats.h"
#include "util.h"

#include <iostream>
using namespace std;

void SceneGame::setGLColorByIndex(int x, int y){
    switch(_board[x][y]){
    case Fog:
        glColor3ub(127,127,127);
        break;
    case Empty:
        glColor3ub(255,255,255);
        break;
    case King:
        if(_selectedCeil.x == x && _selectedCeil.y == y)
            glColor3ub(200,50,200);
        else
            glColor3ub(100,100,255);
        break;
    case EnemyKing:
        glColor3ub(255,0,0);
        break;
    case Soldier:
        if(_selectedCeil.x == x && _selectedCeil.y == y)
            glColor3ub(150,150,0);
        else
            glColor3ub(50,255,50);
        break;
    case EnemySoldier:
        glColor3ub(255,255,0);
        break;
    default: /** ERROR **/
        glColor3ub(0,0,0);
        break;
    }
}

void cheatCallback(const vector<sf::Keyboard::Key> &keys, void* data){
    *(bool*)data = !*(bool*)data;
}

SceneGame::SceneGame(sf::RenderWindow* mainWindow, sf::Vector2u boardSize)
:_mainWindow(mainWindow),_boardSize(boardSize),_nextGamePhase(GameStart),
 _totalPhaseTime(0),_phaseTimeLimit(30000),_maxSoldiers(6),cheatOn(false),_soldiersPlaced(0){
    if(boardSize.x < 2 && boardSize.y < 2)
        throw string("INVALID SCENE ARGUMENT - BOARD SIZE X and Y smaller than 2");
    if(boardSize.x < 2)
        throw string("INVALID SCENE ARGUMENT - BOARD SIZE X smaller than 2");
    if(boardSize.y < 2)
        throw string("INVALID SCENE ARGUMENT - BOARD SIZE Y smaller than 2");

    sf::Vector2u size = _mainWindow->getSize();

    _board = new CeilType*[_boardSize.x];
    for(int i=0; i<_boardSize.x; i++){
        _board[i] = new CeilType[_boardSize.y];
        for(int j=0; j<_boardSize.y; j++)
            _board[i][j] = CeilType::Fog;
    }

    _buttonEndTurn.text = "End turn";
    _buttonEndTurn.x = size.x-200;
    _buttonEndTurn.y = 150;
    _buttonEndTurn.width = 180;
    _buttonEndTurn.height = 50;
    _buttonEndTurn.textColor = sf::Color::White;
    _buttonEndTurn.normalColor = sf::Color(50,150,200);
    _buttonEndTurn.hoverColor = sf::Color(25,75,100);

    _board[_boardSize.x/2][0] = CeilType::EnemyKing;
    _board[_boardSize.x/2][_boardSize.y-1] = CeilType::King;

    sf::Event ev;
    while(_mainWindow->pollEvent(ev));

    glViewport(0, 0, size.x, size.y);
    glOrtho(0,size.x, size.y,0,0,1);
    glMatrixMode(GL_PROJECTION);

    _ch.add({sf::Keyboard::Up,
             sf::Keyboard::Up,
             sf::Keyboard::Down,
             sf::Keyboard::Down,
             sf::Keyboard::Left,
             sf::Keyboard::Right,
             sf::Keyboard::Left,
             sf::Keyboard::Right,
             sf::Keyboard::A,
             sf::Keyboard::B}, &cheatCallback, &cheatOn);
}

SceneGame::~SceneGame(){
    for(int i=0; i<_boardSize.x; i++)
        delete[] _board[i];
    delete[] _board;
}

void SceneGame::render(){
    if(_mainWindow==nullptr)
        return;

    sf::Vector2u size = _mainWindow->getSize();

    if(cheatOn){
        glColor3ub(rand()%256,rand()%256,rand()%256);
        glBegin(GL_POLYGON);
            glVertex2i(0,0);
            glVertex2i(size.x,0);
            glVertex2i(size.x,size.y);
            glVertex2i(0,size.y);
        glEnd();
    }

    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
        glVertex2i(0,0);
        glVertex2i(_boardSize.x*21+1,0);
        glVertex2i(_boardSize.x*21+1,_boardSize.y*21+1);
        glVertex2i(0,_boardSize.y*21+1);
    glEnd();
	for(int i=0; i<_boardSize.x; i++)
        for(int j=0; j<_boardSize.y; j++){
            setGLColorByIndex(i,j);
            glBegin(GL_POLYGON);
                glVertex2i(1+i*21, 1+j*21);
                glVertex2i(1+i*21+20, 1+j*21);
                glVertex2i(1+i*21+20, 1+j*21+20);
                glVertex2i(1+i*21, 1+j*21+20);
            glEnd();
        }
    for(int i=0; i<=1; i++){
        double pos;
        if(i==0)
            pos = _boardSize.y/2*21;
        else
            pos = ceil((double)_boardSize.y/2)*21;
        glColor3ub(255,0,0);
        glBegin(GL_POLYGON);
            glVertex2i(1, pos);
            glVertex2i(_boardSize.x*21, pos);
            glVertex2i(_boardSize.x*21, pos+1);
            glVertex2i(1, pos+1);
        glEnd();
    }

    glColor3ub(0,0,0);
    for(const PieceMove& m:_moves){
        int x = m.to.x;
        int y = m.to.y;
        glBegin(GL_POLYGON);
            glVertex2i(1+x*21, 1+y*21);
            glVertex2i(1+x*21+5, 1+y*21+10);
            glVertex2i(1+x*21, 1+y*21+20);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex2i(1+x*21, 1+y*21+20);
            glVertex2i(1+x*21+10, 1+y*21+15);
            glVertex2i(1+x*21+20, 1+y*21+20);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex2i(1+x*21+20, 1+y*21+20);
            glVertex2i(1+x*21+15, 1+y*21+10);
            glVertex2i(1+x*21+20, 1+y*21);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex2i(1+x*21+20, 1+y*21);
            glVertex2i(1+x*21+10, 1+y*21+5);
            glVertex2i(1+x*21, 1+y*21);
        glEnd();

        glBegin(GL_LINES);
            glVertex2i(1+m.from.x*21+10, 1+m.from.y*21+10);
            glVertex2i(1+x*21+10, 1+y*21+10);
        glEnd();
    }

    _mainWindow->draw(_buttonEndTurn);


    _mainWindow->pushGLStates();

        sf::Text text;
        text.setFont(fontArial);
        text.setColor(sf::Color::White);
        text.setCharacterSize(20);
        switch(_actualGamePhase){
        case GameStart: text.setString("Game Start");break;
        case TurnA: text.setString("Turn A");break;
        case TurnB: text.setString("Turn B");break;
        case GameEnd: text.setString("Game end");break;
        default: text.setString("Invalid Phase");break;
        }
        sf::FloatRect bounds = text.getGlobalBounds();
        text.setPosition(size.x-bounds.width-20, 20);
        _mainWindow->draw(text);
        text.setString(to_string(_phaseTimeLimit<_totalPhaseTime?0:((_phaseTimeLimit-_totalPhaseTime)/1000))+"s remaining");
        bounds = text.getGlobalBounds();
        text.setPosition(size.x-bounds.width-20, 60);
        _mainWindow->draw(text);

        switch(_actualGamePhase){
        case GameStart:
            text.setString(to_string(_maxSoldiers-_soldiersPlaced)+" soldiers remaining...");
            text.setPosition(20, 350);
            _mainWindow->draw(text);
            break;
        }

    _mainWindow->popGLStates();
}

Event SceneGame::process(uint32_t deltaTime){
    sf::Vector2u size = _mainWindow->getSize();

    if(_actualGamePhase != _nextGamePhase)
        _totalPhaseTime =  deltaTime;
    else
        _totalPhaseTime += deltaTime;


    _actualGamePhase = _nextGamePhase;

    switch(_actualGamePhase){
    case GameStart:
        if(processGameStart(deltaTime))
            return Event(EventType::Exit, nullptr);
        break;
    case TurnA:
        if(processTurnA(deltaTime))
            return Event(EventType::Exit, nullptr);
        break;
    case TurnB:
        if(processTurnB(deltaTime))
            return Event(EventType::Exit, nullptr);
        break;
    case GameEnd:
        switch(processGameEnd(deltaTime)){
        case ToMenu:
            return Event(EventType::ChangeScene, new std::pair<SceneType,void*>(SceneType::Menu,nullptr));
        case CloseGame:
            return Event(EventType::Exit, nullptr);
        }
        break;
    }

    _lastGamePhase = _actualGamePhase;

    return Event();
}

bool SceneGame::processGameStart(uint32_t deltaTime){
    sf::Event ev;
    bool ret = false;
    while(_mainWindow->pollEvent(ev)){
        _buttonEndTurn.parseEvent(ev);
        _ch.parseEvent(ev);
        switch(ev.type){
        case sf::Event::KeyPressed:
            if(ev.key.code == sf::Keyboard::Escape){
                _nextGamePhase = PhaseType::GameEnd;
                return ret;
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(ev.mouseButton.button == sf::Mouse::Left){
                int x = mouse.x/21,
                    y = mouse.y/21;
                if(x >= _boardSize.x || y >= _boardSize.y || x < 0 || y < ceil((double)_boardSize.y/2))
                    break;
                if(_board[x][y] == CeilType::Soldier){
                    --_soldiersPlaced;
                    _board[x][y] = CeilType::Fog;
                }else if(_board[x][y] == CeilType::Fog && _soldiersPlaced < _maxSoldiers){
                    ++_soldiersPlaced;
                    _board[x][y] = CeilType::Soldier;
                }
            }
            break;
        case sf::Event::MouseMoved:
            mouse.x = ev.mouseMove.x;
            mouse.y = ev.mouseMove.y;
            break;
        case sf::Event::Closed:
            ret = true;
            break;
        }
    }
    if(_buttonEndTurn.wasPressed() || _totalPhaseTime>_phaseTimeLimit)
        nextGamePhase();
    return ret;
}

bool SceneGame::processTurnA(uint32_t deltaTime){
    if(_lastGamePhase!=_actualGamePhase){
        _selectedCeil.x = _selectedCeil.y = -1;
    }
    sf::Event ev;
    bool ret = false;
    while(_mainWindow->pollEvent(ev)){
        _buttonEndTurn.parseEvent(ev);
        _ch.parseEvent(ev);
        switch(ev.type){
        case sf::Event::KeyPressed:
            if(ev.key.code == sf::Keyboard::Escape){
                _nextGamePhase = PhaseType::GameEnd;
                return ret;
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(ev.mouseButton.button == sf::Mouse::Left){
                int x = mouse.x/21,
                    y = mouse.y/21;
                if(x >= _boardSize.x || y >= _boardSize.y || x < 0 || y < 0)
                    break;
                if(_board[x][y] == CeilType::Soldier || _board[x][y] == CeilType::King){
                    _selectedCeil.x = x;
                    _selectedCeil.y = y;
                }else if(_selectedCeil == sf::Vector2i(x,y)){
                    _selectedCeil.x = _selectedCeil.y = -1;
                }else if(_board[x][y] == CeilType::Fog && _selectedCeil.x != -1 && _selectedCeil.y != -1
                         && abs(x-_selectedCeil.x)<=1 && abs(y-_selectedCeil.y)<=1){
                    bool add = true;
                    for(auto it = _moves.begin(); it!=_moves.end();){
                        if(it->to == sf::Vector2i(x,y)
                        && it->from == _selectedCeil){
                            _moves.erase(it);
                            add = false;
                            break;
                        }else if(it->from == _selectedCeil || it->to == sf::Vector2i(x,y)){
                            it = _moves.erase(it);
                        }else it++;
                    }
                    if(add){
                        _moves.push_front(PieceMove(_selectedCeil, sf::Vector2i(x,y)));
                    }
                    _selectedCeil.x = _selectedCeil.y = -1;
                }
            }
            break;
        case sf::Event::MouseMoved:
            mouse.x = ev.mouseMove.x;
            mouse.y = ev.mouseMove.y;
            break;
        case sf::Event::Closed:
            ret = true;
            break;
        }
    }
    if(_buttonEndTurn.wasPressed() || _totalPhaseTime>_phaseTimeLimit){
        /** TODO: SEND MOVEMENTS TO SERVER **/
        for(const PieceMove& m:_moves){
            _board[m.to.x][m.to.y] = _board[m.from.x][m.from.y];
            _board[m.from.x][m.from.y] = CeilType::Fog;
        }
        _moves.clear();
        nextGamePhase();
    }
    return ret;
}

bool SceneGame::processTurnB(uint32_t deltaTime){
    if(_lastGamePhase!=_actualGamePhase){

    }
    sf::Event ev;
    bool ret = false;
    while(_mainWindow->pollEvent(ev)){
        _buttonEndTurn.parseEvent(ev);
        _ch.parseEvent(ev);
        switch(ev.type){
        case sf::Event::KeyPressed:
            if(ev.key.code == sf::Keyboard::Escape){
                _nextGamePhase = PhaseType::GameEnd;
                return ret;
            }
            break;
        case sf::Event::Closed:
            ret = true;
            break;
        }
    }
    if(_buttonEndTurn.wasPressed() || _totalPhaseTime>_phaseTimeLimit)
        nextGamePhase();
    return ret;
}

EndPhaseReturn SceneGame::processGameEnd(uint32_t deltaTime){
    return ToMenu;
}

void SceneGame::nextGamePhase(){
    switch(_actualGamePhase){
    case GameStart: _nextGamePhase = TurnA; break;
    case TurnA: _nextGamePhase = TurnB; break;
    case TurnB: _nextGamePhase = TurnA; break;
    default: break;
    }
}

void SceneGame::nextGamePhase(PhaseType type){
    _nextGamePhase = type;
}
