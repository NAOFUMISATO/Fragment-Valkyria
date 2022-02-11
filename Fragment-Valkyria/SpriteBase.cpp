
/*****************************************************************//**
 * \file   SpriteBase.cpp
 * \brief  スプライトの基底
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Sprite;

SpriteBase::SpriteBase(Game::GameMain& gameMain) :SpriteBaseRoot{ gameMain }, _gameMain{ gameMain }{
}