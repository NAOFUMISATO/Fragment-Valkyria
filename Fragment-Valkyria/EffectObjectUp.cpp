
#include "EffectObjectUp.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectObjectUp::EffectObjectUp(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectObjectUp::Init() {
   PlayEffect();
}

void EffectObjectUp::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
