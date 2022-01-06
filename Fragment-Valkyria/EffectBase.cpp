#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain) : EffectBaseRoot{ gameMain }, _gameMain{gameMain} {
}

EffectBase::~EffectBase() {
}
