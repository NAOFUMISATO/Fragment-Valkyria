
/*****************************************************************//**
 * \file   ParamBullet.cpp
 * \brief  ãUŒ‚’e‚Ì’lŠÇ—ƒNƒ‰ƒX
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamBullet.h"

using namespace FragmentValkyria::Param;

ParamBullet::ParamBullet(std::string_view key) :ParamBase{key } {
   LoadDoubleParam({"speed" });
}
