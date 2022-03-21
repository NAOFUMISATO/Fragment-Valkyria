
/*****************************************************************//**
 * \file   CreatorBase.cpp
 * \brief  生成管理クラスの基底
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "CreatorBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Create;
using namespace FragmentValkyria;

CreatorBase::CreatorBase(Game::GameMain& gameMain) :_gameMain{gameMain} {
}

AppFrame::Resource::LoadResourceJson& CreatorBase::GetLoadJson() const {
   return _gameMain.loadresJson();
}
