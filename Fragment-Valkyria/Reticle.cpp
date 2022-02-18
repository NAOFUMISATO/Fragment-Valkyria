#include "Reticle.h"
#include "GameMain.h"
#include "Player.h"
#include "ObjectServer.h"
#include "PlayerHP.h"

using namespace FragmentValkyria::Player;

Reticle::Reticle(Game::GameMain& gameMain) : Sprite::SpriteBase{ gameMain }{

}

void Reticle::Init() {
   _grHandle = GetResServer().GetTexture("Reticle");
   _position = { 0,0,0 };
}

void Reticle::Update() {
   for (auto&& object : _gameMain.objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player&>(*object);
         _isAim = player.isAim();
      }
   }
}

void Reticle::Draw() {
   if (_isAim) {
      SpriteBase::Draw();
   }
}
