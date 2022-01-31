#include "RemainingBullet.h"
#include "GameMain.h"

namespace{
}

using namespace FragmentValkyria::Player;

RemainingBullet::RemainingBullet(Game::GameMain& gameMain) :Sprite::SpriteBase{gameMain} {
}

void RemainingBullet::Init() {
   _grHandles = GetResServer().GetTextures("RemainingBullet");
   _position = {30,150,0};
}

void RemainingBullet::Update() {

}

void RemainingBullet::Draw() {

}
