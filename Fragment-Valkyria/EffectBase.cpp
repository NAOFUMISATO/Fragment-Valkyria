
/*****************************************************************//**
 * \file   EffectBase.cpp
 * \brief  エフェクトの基底
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain,std::string_view key) : EffectBaseRoot{ gameMain,key }, _gameMain{gameMain} {
}
