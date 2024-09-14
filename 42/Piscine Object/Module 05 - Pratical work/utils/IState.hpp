#ifndef STATE_H_
#define STATE_H_

class IState {
public:
  virtual ~IState() = default;
  virtual void Update() = 0;
  virtual void Draw() = 0;
};

#endif // IGAME_STATE_H_