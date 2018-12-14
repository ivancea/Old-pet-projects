#ifndef SCENEGAME_H
#define SCENEGAME_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include <list>

#include "cheats.h"
#include "util.h"
#include "scene.h"

enum CeilType:char{
    Fog = -1,
    Empty = 0,
    King = 1,
    EnemyKing,
    Soldier,
    EnemySoldier
};
enum PhaseType{
    GameStart = 0,
    TurnA = 1,
    TurnB = 2,
    GameEnd
};

enum EndPhaseReturn{
    Again = 0,
    ToMenu = 1,
    CloseGame = 2
};

class SceneGame:public Scene{
    sf::RenderWindow* _mainWindow;
    sf::Vector2u mouse;

    CeilType** _board;
    sf::Vector2u _boardSize;

    PhaseType _actualGamePhase,
              _lastGamePhase,
              _nextGamePhase;

    int _totalPhaseTime;
    int _phaseTimeLimit;

    int _maxSoldiers;

    sf::Vector2i _selectedCeil;
    std::list<PieceMove> _moves;

    Button _buttonEndTurn;

    bool cheatOn;
    Cheats _ch;

    /// TURN DATA
    int _soldiersPlaced;

    void setGLColorByIndex(int x, int y);

    void nextGamePhase();
    void nextGamePhase(PhaseType type);

    bool processGameStart(uint32_t deltaTime);
    bool processTurnA(uint32_t deltaTime);
    bool processTurnB(uint32_t deltaTime);
    EndPhaseReturn processGameEnd(uint32_t deltaTime);

public:
    SceneGame(sf::RenderWindow* mainWindow, sf::Vector2u boardSize);
    virtual ~SceneGame();

    virtual Event process(uint32_t deltaTime);
    virtual void render();
};

#endif // SCENEGAME_H
