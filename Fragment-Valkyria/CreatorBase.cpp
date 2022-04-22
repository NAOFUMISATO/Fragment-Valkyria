
/*****************************************************************//**
 * \file   CreatorBase.cpp
 * \brief  ¶¬ŠÇ—ƒNƒ‰ƒX‚ÌŠî’ê
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "CreatorBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Create;
using namespace FragmentValkyria;

AppFrame::Resource::LoadResourceJson& CreatorBase::GetLoadJson() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->loadresJson();
}
