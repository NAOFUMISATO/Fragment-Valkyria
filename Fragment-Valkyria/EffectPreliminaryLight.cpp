#include "EffectPreliminaryLight.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPreliminaryLight::EffectPreliminaryLight(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectPreliminaryLight::Init() {
   PlayEffect();
}

void EffectPreliminaryLight::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
