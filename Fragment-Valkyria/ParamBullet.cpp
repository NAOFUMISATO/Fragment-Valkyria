
/*****************************************************************//**
 * \file   ParamBullet.cpp
 * \brief  ãUŒ‚’e‚Ì’lŠÇ—ƒNƒ‰ƒX
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamBullet.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamBullet::ParamBullet(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadDoubleParam({"speed" });
}
