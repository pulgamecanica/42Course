#ifndef VISUAL_NODE_HPP
#define VISUAL_NODE_HPP

#include "Graph.hpp" // Node is defined here
#include "raylib.h"

enum class NodeState {
  NORMAL,
  HOVERED,
  SELECTED
};

class VisualNode {
public:
  VisualNode(Node* node, Vector2 position, float size, Color color);

  void Draw(Vector2 gridOffset, float zoom);
  void UpdatePosition(Vector2 newPosition);
  Vector2 GetPosition() const;
  Vector2 GetAbsolutePosition(Vector2 gridOffset, float zoom) const;
  Node* GetNode() const;
  bool IsMouseOver(Vector2 mousePosition, Vector2 gridOffset, float zoom) const;
  void DrawID(Vector2 gridOffset, float zoom);
  void SetState(NodeState state);
  NodeState GetState() const;
  void SetSize(float size);

private:
  Node* node_;
  Vector2 position_;
  float size_;
  Color color_;
  NodeState state_;
};

#endif // VISUAL_NODE_HPP
