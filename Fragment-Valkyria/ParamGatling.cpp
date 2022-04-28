
/*****************************************************************//**
 * \file   ParamGatling.cpp
 * \brief  �K�g�����O�̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamGatling.h"

using namespace FragmentValkyria::Param;

ParamGatling::ParamGatling(std::string_view key) :ParamBase{key } {
   LoadDoubleParam({ "speed" });
}
