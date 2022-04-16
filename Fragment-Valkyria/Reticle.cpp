
/*****************************************************************//**
 * \file   Reticle.cpp
 * \brief  ƒŒƒeƒBƒNƒ‹
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "Reticle.h"
#include "GameMain.h"
#include "Player.h"
#include "ObjectServer.h"
#include "PlayerHP.h"
#include "ParamPlayerUI.h"

namespace {
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Player;

Reticle::Reticle(Game::GameMain& gameMain) : Sprite::SpriteBase{ gameMain }{
   _param = std::make_unique<Param::ParamPlayerUI>(_gameMain,"playerui");
}

void Reticle::Init() {
   _grHandles = GetResServer().GetTextures("Reticle");
   _position = _param->GetVecParam("reticle_pos");
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
      GetTexComponent().DrawTexture(static_cast<int>(x), static_cast<int>(y),
         DefaultScale, DefaultAngle, _grHandles, _param->GetIntParam("reticle_animespeed"));
   }
}
