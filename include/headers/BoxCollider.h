#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_

#include <QuickSDL/GameEntity.h>
#include <headers/Collider.h>

using namespace QuickSDL;

class BoxCollider : public Collider {
 private:
  static const int MAX_VERTS = 4;

  GameEntity* mVerts[MAX_VERTS];

  Vector2 mSize;

 public:
  BoxCollider(Vector2 size);
  ~BoxCollider();

  Vector2 GetFurthestPoint() override;

  Vector2 GetVertexPos(int index);

  Vector2 GetSize();

 private:
  void AddVert(int index, Vector2 pos);
};

#endif
