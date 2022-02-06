
/*****************************************************************//**
 * \file   BossStageCreator.cpp
 * \brief  ステージの生成
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "StageCreator.h"
#include "Stage.h"
#include "GameMain.h"
#include "ObjectBase.h"
#include "LightAndShadow.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

StageCreator::StageCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> StageCreator::Create() {
   /// ステージの生成
   auto stage = std::make_unique<Stage::Stage>(_gameMain);
   return std::move(stage);
}