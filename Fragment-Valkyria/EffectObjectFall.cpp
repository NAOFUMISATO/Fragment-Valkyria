
#include "EffectObjectFall.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectObjectFall::EffectObjectFall(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectObjectFall::Init() {
   PlayEffect();
}

void EffectObjectFall::Update() {
   EffectBase::Update();
   if (!IsPlaying()) {
      SetDead();
   }
}
