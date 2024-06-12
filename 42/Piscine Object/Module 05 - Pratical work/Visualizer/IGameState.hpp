#ifndef GAME_STATE_H
#define GAME_STATE_H

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

#endif // IGAME_STATE_H
