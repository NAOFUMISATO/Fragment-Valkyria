
/*****************************************************************//**
 * \file   Stage.cpp
 * \brief  ステージ
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "BossStage.h"
#include "ModelComponent.h"
#include "ObjectServer.h"
#include "StageModelComponent.h"
using namespace FragmentValkyria::Stage;

BossStage::BossStage(Game::GameMain& game) : Object::ObjectBase{ game } {
   // スカイスフィアのモデル
   _skySphere = std::make_unique<Model::ModelComponent>(*this);
   _skySphere->SetModel("SkySphere");
   _skySphere->SetScale({ 80.f,  80.f, 80.f });

   _stage = std::make_unique<StageModelComponent>(*this);
   _stage->SetModels("Boss");
   _stage->PixelLightingON();
}

void BossStage::Update() {
   // スカイスフィアをプレイヤと同じ位置にする
   _skySphere->SetPosition(GetObjServer().GetVecData("PlayerPos"));
}

void BossStage::Draw() {
   _skySphere->Draw(); // スカイスフィア
   _stage->Draw();
}
