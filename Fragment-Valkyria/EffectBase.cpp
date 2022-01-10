#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain) : EffectBaseRoot{ gameMain }, _gameMain{gameMain} {
}

EffectBase::~EffectBase() {
}

void EffectBase::Init(std::string_view key) {
   EffectBaseRoot::Init(key);
}

void EffectBase::Draw() {
   SetPosition(_position);
   SetRotation(_rotation);
   SetScale(_scale);
   SetSpeed(_speed);
   SetEfcColor(_color);
}
