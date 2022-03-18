#include "EffectObjectHit.h"

using namespace FragmentValkyria::Effect;

EffectObjectHit::EffectObjectHit(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectObjectHit::Init() {
   PlayEffect();
}

void EffectObjectHit::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
