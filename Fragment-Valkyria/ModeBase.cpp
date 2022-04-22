
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  Šeƒ‚[ƒh‚ÌŠî’ê
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "GameMain.h"
#include "ObjectServer.h"
#include "SpriteServer.h"

using namespace FragmentValkyria::Mode;

ModeBase::ModeBase() {
}

FragmentValkyria::Object::ObjectServer& ModeBase::GetObjServer() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->objServer();
}

FragmentValkyria::Sprite::SpriteServer& ModeBase::GetSprServer() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->sprServer();
}

FragmentValkyria::Effect::EffectServer& ModeBase::GetEfcServer() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->efcServer();
}