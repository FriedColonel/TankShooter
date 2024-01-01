//-------------------------------------------------------------------------------------------------------------------//
// GameEntity.cpp // The base class for all game entities, providing position,
// rotation, and scaling functionality to all entities.    //
//                                                                                                                   //
// Sets up a system to parent GameEntity objects to one another // making the
// child's position, rotation, and scale relative to the parent's instead of the
// world origin             //
//                                                                                                                   //
// By: Ather Omar //
//-------------------------------------------------------------------------------------------------------------------//
#include <QuickSDL/GameEntity.h>
//-------------------------------------------------------------
// QuickSDL
//-------------------------------------------------------------
namespace QuickSDL {

GameEntity::GameEntity(Vector2 pos) {
  mPos = pos;
  mRotation = 0.0f;
  mActive = true;
  mParent = NULL;
  mScale = VEC2_ONE;
}

GameEntity::~GameEntity() { mParent = NULL; }

void GameEntity::Pos(Vector2 pos) { mPos = pos; }

Vector2 GameEntity::Pos(SPACE space) {
  if (space == local || mParent == NULL) return mPos;

  GameEntity* parent = mParent;
  Vector2 finalPos = mPos, parentScale = VEC2_ZERO;

  do {
    Vector2 parentScale = parent->Scale(local);
    finalPos = RotateVector(
        Vector2(finalPos.x * parentScale.x, finalPos.y * parentScale.y),
        parent->Rotation(local));
    finalPos += parent->Pos(local);

    parent = parent->Parent();
  } while (parent);

  // Vector2 parentScale = mParent->Scale(world);
  // // The object's local position is rotated by the parent's rotation
  // Vector2 rotPos =
  //     RotateVector(Vector2(mPos.x * parentScale.x, mPos.y * parentScale.y),
  //                  mParent->Rotation(local));

  // The final position also depends on the parent's scale (if the parent is
  // scaled up, the object should be further away from the parent)
  // return mParent->Pos(world) + rotPos;
  return finalPos;
}

void GameEntity::Rotation(float r) {
  mRotation = r;

  // Wraps the angle between 0 and 360 degrees, addition and subtraction is sed
  // to avoid snapping Updated to deal with degrees higher than 360 and -360
  if (mRotation > 360.0f) {
    int mul = mRotation / 360;
    mRotation -= 360.0f * mul;

  } else if (mRotation < 0.0f) {
    int mul = (mRotation / 360) - 1;
    mRotation -= 360.0f * mul;
  }
}

float GameEntity::Rotation(SPACE space) {
  if (space == local || mParent == NULL) return mRotation;

  return mParent->Rotation(world) + mRotation;
}

void GameEntity::Scale(Vector2 scale) { mScale = scale; }

Vector2 GameEntity::Scale(SPACE space) {
  if (space == local || mParent == NULL) return mScale;

  Vector2 scale = mParent->Scale(world);
  scale.x *= mScale.x;
  scale.y *= mScale.y;

  return scale;
}

void GameEntity::Active(bool active) { mActive = active; }

bool GameEntity::Active() { return mActive; }

void GameEntity::Parent(GameEntity* parent) {
  // If the parent is removed, The Position/Rotation/Scale are the GameEntity's
  // world values, to keep the object looking the same after the removal of the
  // parent;
  if (parent == NULL) {
    mPos = Pos(world);
    mScale = Scale(world);
    mRotation = Rotation(world);

  } else {
    // If the object already has a parent, remove the current parent to get it
    // ready to be the child for the new parent
    if (mParent != NULL) Parent(NULL);

    Vector2 parentScale = parent->Scale(world);

    // Setting the local position to be relative to the new parent (while
    // maintaining the same world position as before)
    mPos =
        RotateVector(Pos(world) - parent->Pos(world), -parent->Rotation(world));
    mPos.x /= parentScale.x;
    mPos.y /= parentScale.y;

    // Setting the local rotation to be relative to the new parent (while
    // maintaining the same world rotation as before)
    mRotation = mRotation - parent->Rotation(world);

    // Setting the new scale to be relative to the new parent (while maintaining
    // the same world scale as before)
    mScale = Vector2(mScale.x / parentScale.x, mScale.y / parentScale.y);
  }

  mParent = parent;
}

GameEntity* GameEntity::Parent() { return mParent; }

void GameEntity::Translate(Vector2 vec, SPACE space) {
  if (space == world) {
    mPos += vec;
  } else {
    mPos += RotateVector(vec, Rotation());
  }

  mPos += vec;
}

void GameEntity::Rotate(float amount) { mRotation += amount; }

void GameEntity::AbsoluteRotate(float amount, SPACE space) {
  mRotation = amount;

  if (space == local && mParent != NULL) {
    mRotation -= mParent->Rotation(world);
  } else {
    mRotation = amount;
  }
}

void GameEntity::RotateWorld(float amount) {
  // rotate the object without care for the parent's rotation
  mRotation = amount;

  // if the object has a parent, subtract the parent's rotation to keep the
  // object's world rotation the same
  if (mParent != NULL) mRotation -= mParent->Rotation(world);
}

void GameEntity::Update() {}

void GameEntity::Render() {}
}  // namespace QuickSDL
