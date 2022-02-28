
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  äeÉÇÅ[ÉhÇÃäÓíÍ
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "GameMain.h"
#include "ObjectServer.h"
#include "SpriteServer.h"

using namespace FragmentValkyria::Mode;

ModeBase::ModeBase(Game::GameMain& gameMain) : ModeBaseRoot{ gameMain }, _gameMain{ gameMain } {
}

FragmentValkyria::Object::ObjectServer& ModeBase::GetObjServer() const {
   return _gameMain.objServer();
}

FragmentValkyria::Sprite::SpriteServer& ModeBase::GetSprServer() const {
   return _gameMain.sprServer();
}

FragmentValkyria::Effect::EffectServer& ModeBase::GetEfcServer() const {
   return _gameMain.efcServer();
}