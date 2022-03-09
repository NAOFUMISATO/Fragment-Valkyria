#include "Reticle.h"
#include "GameMain.h"
#include "Player.h"
#include "ObjectServer.h"
#include "PlayerHP.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", {"reticle_animespeed"});
   const int ReticleAnimeSpeed = paramMap["reticle_animespeed"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "reticle_pos" });
   const auto ReticlePos = vecParamMap["reticle_pos"];

   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Player;

Reticle::Reticle(Game::GameMain& gameMain) : Sprite::SpriteBase{ gameMain }{

}

void Reticle::Init() {
   _grHandles = GetResServer().GetTextures("Reticle");
   _position = ReticlePos;
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
      auto [x, y, z] = _position.GetVec3();
      GetTexComponent().DrawTexture(x, y, DefaultScale, DefaultAngle, _grHandles, ReticleAnimeSpeed);
   }
}
