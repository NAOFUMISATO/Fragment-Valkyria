
/*****************************************************************//**
 * \file   BossStageCreator.cpp
 * \brief  ステージの生成
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
   /// ステージの生成
   auto stage = std::make_unique<Stage::BossStage>(_gameMain);
   return std::move(stage);
}