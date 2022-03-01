#include "EffectWeakBullet.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectWeakBullet::EffectWeakBullet(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectWeakBullet::Init() {
   PlayEffect();
}

void EffectWeakBullet::Update() {
   EffectBase::Update();
   if (!IsPlaying()) {
      SetDead();
   }
}


