#include "EffectBossWalk.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossWalk::EffectBossWalk(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossWalk::Init() {
   PlayEffect();
}

void EffectBossWalk::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

