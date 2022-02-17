#include "Reticle.h"
#include "GameMain.h"
#include "Player.h"
using namespace FragmentValkyria::Player;

Reticle::Reticle(Game::GameMain& gameMain) : Sprite::SpriteBase{ gameMain }{

}

void Reticle::Init() {
   _grHandle = GetResServer().GetTexture("Reticle");
   _position = { 0,0,0 };
}

void Reticle::Update() {
   for (auto&& object : _gameMain.objServer().runObjects()) {
      auto& obj = dynamic_cast<Object::ObjectBase&>(*object);
      if (obj.GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player&>(obj);
         _isAim = player.isAim();
      }
   }
}

void Reticle::Draw() {
   if (_isAim) {
      SpriteBase::Draw();
   }
}
