
/*****************************************************************//**
 * \file   ParamModeTitle.cpp
 * \brief  タイトルの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamModeTitle.h"

using namespace FragmentValkyria::Param;

ParamModeTitle::ParamModeTitle(std::string_view key) :ParamBase{key} {
   LoadIntParam({ "bg_x","bg_y","title_x","title_y",
      "title_animespeed","anybutton_x" ,"anybutton_y",
      "anybutton_animespeed","start_x","start_y",
      "start_animespeed","option_x","option_y",
      "option_animespeed","end_x","end_y","end_animespeed" });
}
