
/*****************************************************************//**
 * \file   ParamGatling.cpp
 * \brief  ガトリングの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamGatling.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamGatling::ParamGatling(std::string_view key) :ParamBase{key } {
   LoadDoubleParam({ "speed" });
}
