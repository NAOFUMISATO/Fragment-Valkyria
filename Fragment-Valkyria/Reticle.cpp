
/*****************************************************************//**
 * \file   Reticle.cpp
 * \brief  ƒŒƒeƒBƒNƒ‹
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "Reticle.h"
#include "Game.h"
#include "Player.h"
#include "ObjectServer.h"
#include "PlayerHP.h"
#include "ParamPlayerUI.h"

namespace {
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Player;

Reticle::Reticle() {
   _param = std::make_unique<Param::ParamPlayerUI>("playerui");
}

void Reticle::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandles = resServer.GetTextures("Reticle");
   _position = _param->GetVecParam("reticle_pos");
}

void Reticle::Update() {
   auto& runObjects = Game::Game::GetInstance().objServer().runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player&>(*object);
         _isAim = player.isAim();
      }
   }
}

void Reticle::Draw() {
   if (_isAim) {
      auto [x, y, z] = _position.GetVec3();
      auto& texComponent = Game::Game::GetInstance().texComponent();
      texComponent.DrawTexture(static_cast<int>(x), static_cast<int>(y),
         DefaultScale, DefaultAngle, _grHandles, _param->GetIntParam("reticle_animespeed"));
   }
}
