
#include "EffectBossBeam.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossBeam::EffectBossBeam(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossBeam::Init() {
   PlayEffect();
}

void EffectBossBeam::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
