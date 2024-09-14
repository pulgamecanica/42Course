#ifndef __ANIMATION_HPP__
# define __ANIMATION_HPP__

#include <iostream>
#include <vector>
#include <map>
#include "raylib.h"

struct AnimationFrame {
  Texture2D texture;
  float duration;  // Duration of the frame in seconds
};

class Animation {
public:
  Animation(const std::vector<std::string>& imagePaths, float frameDuration, std::map<std::string, int> options = {});
  Animation(const std::vector<const char*>& imagePaths, float frameDuration, std::map<std::string, int> options = {});

  ~Animation();
  void Update();
  void Draw(int x, int y);
  
private:
  std::vector<AnimationFrame> frames_;
  int   current_frame_;
  int   tint_frames_cycle_; // How many frames per cycle of tint, from light to dark to light
  float elapsed_time_;
};
#endif // __ANIMATION_HPP__