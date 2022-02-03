
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

#ifndef _DEBUG
   auto [charaCollHandle, charaCollNum] = _stage->GetHandleAndCollNum("stage_character_c");
   auto [bossCollHandle, bossCollNum] = _stage->GetHandleAndCollNum("stage_boss_c");
   auto [objCollHandle, objCollNum] = _stage->GetHandleAndCollNum("stage_object_c");
   MV1SetFrameVisible(charaCollHandle, charaCollNum, false);
   MV1SetFrameVisible(bossCollHandle, bossCollNum, false);
   MV1SetFrameVisible(objCollHandle, objCollNum, false);
#else
   auto [charaCollHandle,charaCollNum] = _stage->GetHandleAndCollNum("stage_character_c");
   auto [bossCollHandle,bossCollNum] = _stage->GetHandleAndCollNum("stage_boss_c");
   auto [objCollHandle,objCollNum] = _stage->GetHandleAndCollNum("stage_object_c");
   MV1SetFrameOpacityRate(charaCollHandle, charaCollNum,0.6f);
   MV1SetFrameOpacityRate(bossCollHandle, bossCollNum, 1.0f);
   MV1SetFrameOpacityRate(objCollHandle, objCollNum, 0.2f);
#endif
}

void BossStage::Update() {
   // スカイスフィアをプレイヤと同じ位置にする
   _skySphere->SetPosition(GetObjServer().GetVecData("PlayerPos"));
}

void BossStage::Draw() {
   _skySphere->Draw(); // スカイスフィア
   _stage->Draw();
}
