
#include "EffectGatlingBullet.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectGatlingBullet::EffectGatlingBullet(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectGatlingBullet::Init() {
   PlayEffect();
}

void EffectGatlingBullet::Update() {
   if (!IsPlaying()) {
      PlayEffect();
   }
}
