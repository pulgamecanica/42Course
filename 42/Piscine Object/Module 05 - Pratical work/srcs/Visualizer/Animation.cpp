//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Visualizer/Animation.hpp"


/**
 * Options accepted:
 * If you want to resize the image, you can pass the following options:
 * width: int
 * height: int
 * tint frames cycle: int // If not provided, default: -1 (disabled, no tint)
 **/
Animation::Animation(const std::vector<const char*>& imagePaths, float frameDuration, std::map<std::string, int> options) {
  current_frame_ = 0;
  elapsed_time_ = 0.0f;
  tint_frames_cycle_ = options.contains("tint frames cycle") ? options["tint frames cycle"] : -1;

  for (const char* path : imagePaths) {
    Image img = LoadImage(path);
    if (img.data == nullptr) {
      throw std::runtime_error(std::string("Failed to load image: ") + path);
    }
    if (!options["width"] || !options["height"])
      throw std::runtime_error("Width or Height cannot be zero!");
    ImageResizeNN(&img, options["width"], options["height"]);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    frames_.push_back({ texture, frameDuration });
  }
}

// Animation::Animation(const char* spriteSheet, float frameDuration) {
//   current_frame_ = 0;
//   elapsed_time_ = 0.0f;
// }

Animation::~Animation() {
  for (AnimationFrame& frame : frames_)
    UnloadTexture(frame.texture);
}

void Animation::UpdateAnimation(float delta_time) {
  if (frames_.empty()) return;

  elapsed_time_ += delta_time;
  if (elapsed_time_ >= frames_[current_frame_].duration) {
    elapsed_time_ = 0.0f;
    current_frame_ = (current_frame_ + 1) % frames_.size();
  }
}

Color CalculateTint(int framesPerPhase) {
  // Going from Light to dark to light again
  if (framesPerPhase == -1)
    return WHITE;
  std::vector<Color> tints;
  for (int i = 255; i > 42; --i)
    tints.push_back(Color(i, i, i, 255));
  for (int i = 42; i < 255; ++i)
    tints.push_back(Color(i, i, i, 255));

  // float deltaTime = GetFrameTime();

  int currentFrame = (int)(GetTime() * 60) % framesPerPhase;  // Assuming 60 FPS
  float ratio = currentFrame / (float)framesPerPhase;
  int tintIndex = ratio * tints.size(); 
  return tints[tintIndex];
}

void Animation::DrawAnimation(int x, int y) {
  if (frames_.empty()) return;

  Color tint = CalculateTint(tint_frames_cycle_);
  DrawTexture(frames_[current_frame_].texture, x, y, tint);
}
