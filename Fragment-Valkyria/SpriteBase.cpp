
/*****************************************************************//**
 * \file   SpriteBase.cpp
 * \brief  �X�v���C�g�̊��
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Sprite;

SpriteBase::SpriteBase(Game::GameMain& gameMain) :SpriteBaseRoot{ gameMain }, _gameMain{ gameMain }{
}