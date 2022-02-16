
/*****************************************************************//**
 * \file   EffectBase.cpp
 * \brief  �G�t�F�N�g�̊��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain,std::string_view key) : EffectBaseRoot{ gameMain,key }, _gameMain{gameMain} {
}
