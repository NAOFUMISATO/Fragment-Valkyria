#include "EffectBossDieAfter.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossDieAfter::EffectBossDieAfter(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossDieAfter::Init() {
   PlayEffect();
   _efcCnt = _gameMain.modeServer().frameCount();
   EffectBase::Update();
}

void EffectBossDieAfter::Update() {
   auto frame = _gameMain.modeServer().frameCount() - _efcCnt;
   if (frame % 120 == 0) {
      PlayEffect();
   }
   EffectBase::Update();
}

