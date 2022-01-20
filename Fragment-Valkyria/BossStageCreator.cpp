
/*****************************************************************//**
 * \file   BossStageCreator.cpp
 * \brief  �X�e�[�W�̐���
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "BossStageCreator.h"
#include "BossStage.h"
#include "GameMain.h"
#include "ObjectBase.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

BossStageCreator::BossStageCreator(Game::GameMain& gameMain) : CreatorBase{gameMain} {

}

std::unique_ptr<Object::ObjectBase> BossStageCreator::Create() {
   /// �X�e�[�W�̐���
   auto stage = std::make_unique<Stage::BossStage>(_gameMain);
   return std::move(stage);
}