
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  Šeƒ‚[ƒh‚ÌŠî’ê
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "GameMain.h"
#include "ObjectBase.h"

using namespace FragmentValkyria::Mode;

ModeBase::ModeBase(Game::GameMain& gameMain) : ModeBaseRoot{ gameMain }, _gameMain{ gameMain } {
}

