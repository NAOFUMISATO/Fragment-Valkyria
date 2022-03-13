#include "EffectBossFall.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossFall::EffectBossFall(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossFall::Init() {
   PlayEffect();
}

void EffectBossFall::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

