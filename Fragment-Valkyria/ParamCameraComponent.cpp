
/*****************************************************************//**
 * \file   ParamCameraComponent.cpp
 * \brief  カメラの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamCameraComponent.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamCameraComponent::ParamCameraComponent( std::string_view key) :ParamBase{key } {
   LoadIntParam({ "dead_zone_divide_value"});
   LoadDoubleParam({ "center_height", "spring_k","divide_t" });
}
