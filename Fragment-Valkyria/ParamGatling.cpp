
/*****************************************************************//**
 * \file   ParamGatling.cpp
 * \brief  �K�g�����O�̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamGatling.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamGatling::ParamGatling(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadDoubleParam({ "speed" });
}
