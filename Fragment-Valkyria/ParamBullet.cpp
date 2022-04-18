
/*****************************************************************//**
 * \file   ParamBullet.cpp
 * \brief  ��U���e�̒l�Ǘ��N���X
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
