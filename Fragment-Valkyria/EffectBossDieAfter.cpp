#include "EffectBossDieAfter.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossDieAfter::EffectBossDieAfter(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossDieAfter::Init() {
   PlayEffect();
}

void EffectBossDieAfter::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

