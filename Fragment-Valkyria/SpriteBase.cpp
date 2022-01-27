#include "SpriteBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Sprite;

SpriteBase::SpriteBase(Game::GameMain& gameMain) :SpriteBaseRoot{ gameMain }, _gameMain{ gameMain }{
}
SpriteBase::~SpriteBase() {
}