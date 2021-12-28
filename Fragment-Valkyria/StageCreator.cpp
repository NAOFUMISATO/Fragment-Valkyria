
/*****************************************************************//**
 * \file   StageCreator.cpp
 * \brief  ステージの生成
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "StageCreator.h"
#include "Stage.h"
#include "GameMain.h"
#include "ObjectBase.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> StageCreator::Create(Game::GameMain& gameMain) {
   /// ステージの生成
   auto stage = std::make_unique<Stage::Stage>(gameMain);
   return std::move(stage);
}