//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Visualizer.inc"

#include "raylib.h"

// #define RAYGUI_IMPLEMENTATION // Not needed because I use the shared library
#include "raygui.h"

#include <iostream>
#include <vector>

#define WIDTH 900
#define HEIGHT 550


struct AnimationFrame {
  Texture2D texture;
  float duration;  // Duration of the frame in seconds
};

struct Animation {
  std::vector<AnimationFrame> frames;
  int currentFrame;
  float elapsedTime;
};

Animation LoadAnimation(const std::vector<const char*>& imagePaths, float frameDuration) {
  Animation anim;
  anim.currentFrame = 0;
  anim.elapsedTime = 0.0f;

  for (const char* path : imagePaths) {
    Image img = LoadImage(path);
    if (img.data == nullptr) {
      std::cerr << "Failed to load image: " << path << std::endl;
      continue;
    }
    ImageResizeNN(&img, WIDTH, HEIGHT);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    anim.frames.push_back({ texture, frameDuration });
  }

  return anim;
}

void UnloadAnimation(Animation& anim) {
  for (AnimationFrame& frame : anim.frames) {
    UnloadTexture(frame.texture);
  }
}

void UpdateAnimation(Animation& anim, float deltaTime) {
  if (anim.frames.empty()) return;

  anim.elapsedTime += deltaTime;
  if (anim.elapsedTime >= anim.frames[anim.currentFrame].duration) {
    anim.elapsedTime = 0.0f;
    anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
  }
}

void DrawAnimation(Animation& anim, int x, int y, Color tint) {
  if (anim.frames.empty()) return;

  DrawTexture(anim.frames[anim.currentFrame].texture, x, y, tint);
}

Color CalculateTint(int frame, int framesPerPhase) {
  std::vector<Color> tints = {
    Color(255, 255, 255, 255),
    Color(255, 255, 255, 255),
    Color(200, 242, 221, 255),
    Color(200, 200, 221, 255),
    Color(142, 142, 180, 255),
    Color(120, 120, 142, 255),
    Color(84, 84, 94, 255),
    Color(42, 42, 72, 255),
    Color(21, 21, 41, 255),
    Color(42, 42, 72, 255),
    Color(84, 84, 94, 255),
    Color(120, 120, 142, 255),
    Color(255, 255, 255, 200),
    Color(242, 242, 242, 242),
  };

  frame %= framesPerPhase;
  float ratio = frame / (float)framesPerPhase;
  int tintIndex = ratio * tints.size(); 
  return tints[tintIndex];
}

bool showMessageBox = false;

void GUI() {
  if (GuiButton(Rectangle(24, 24, 120, 30 ), "#191#Show Message"))
      showMessageBox = true;

  if (showMessageBox) {
    int result = GuiMessageBox(Rectangle(85, 70, 250, 100),
        "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

    if (result >= 0)
      showMessageBox = false;
  }
}

int main() {
  // Initialization
  const int screenWidth = WIDTH;
  const int screenHeight = HEIGHT;
  
  InitWindow(screenWidth, screenHeight, "raylib - Dynamic Animated Background");

  std::vector<const char*> backgrounds = { "images/background.png", "images/background2.png", "images/background3.png" };

  Animation anim1 = LoadAnimation(backgrounds, 42.0f);  // 0.42 frames per second

  int totalFrames = 4200;
  int framesPerImage = totalFrames / 3;


  while (!WindowShouldClose()) {
      // Update
      float deltaTime = GetFrameTime();

      // Determine current frame and image
      int currentFrame = (GetTime() * 30);  // Assuming 60 FPS

      // Calculate tint
      Color tint = CalculateTint(currentFrame, framesPerImage);

      // Update animations
      UpdateAnimation(anim1, deltaTime);

      // Start drawing
      BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw current frame with tint
      DrawAnimation(anim1, 0, 0, tint);

      // Draw Raygui
      GUI();

      // Finish drawing
      EndDrawing();
  }

  // De-Initialization
  UnloadAnimation(anim1);
  CloseWindow();

  return 0;
}
