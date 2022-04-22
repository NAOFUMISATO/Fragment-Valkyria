
/*****************************************************************//**
 * \file   ParamModeOption.cpp
 * \brief  �I�v�V������ʂ̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamModeOption.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamModeOption::ParamModeOption(std::string_view key) :ParamBase{key } {
   LoadIntParam({"default_deadzone","deadzone_min","deadzone_max","aimsens_x" ,"aimsens_y" ,
      "aimbar_x","aimbar_y","camerasens_x","camerasens_y","camerabar_x","camerabar_y","deadzone_x" ,"deadzone_y" ,
      "deadzonebar_x","deadzonebar_y","return_x" ,"return_y" ,"bg_alpha" });
   LoadDoubleParam({ "default_camera_sens","default_aim_sens", "sensivity_min","sensivity_max" });
}