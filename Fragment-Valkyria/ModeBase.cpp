
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  �e���[�h�̊��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Mode;

ModeBase::ModeBase(Game::GameMain& gameMain) : ModeBaseRoot{ gameMain }, _gameMain{ gameMain } {
}

ModeBase::~ModeBase() {
}

