
#include "EffectObjFall.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectObjFall::EffectObjFall(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectObjFall::Init() {
   PlayEffect();
}

void EffectObjFall::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
}
