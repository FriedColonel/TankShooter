#include <headers/Collider.h>

#include <iostream>

Collider::Collider(ColliderType type) : mType(type) { mDebugTexture = nullptr; }

Collider::~Collider() {
  if (mDebugTexture) {
    delete mDebugTexture;
    mDebugTexture = nullptr;
  }
}

void Collider::SetDebugTexture(Texture* debugTexture) {
  delete mDebugTexture;
  mDebugTexture = debugTexture;

  mDebugTexture->Parent(this);
}

Collider::ColliderType Collider::GetType() { return mType; }

void Collider::Render() {
  if (mDebugTexture) {
    mDebugTexture->Render();
  }
}
