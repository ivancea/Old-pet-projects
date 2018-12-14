#ifndef STATE_H
#define STATE_H

enum Status{
    Error = -1,
    Offline = 0,
    Online = 1,
    Busy = 2,
    Away = 3,
    Snooze = 4,
    LookingToTrade = 5,
    LookingToPlay = 6
};

#endif // STATE_H
