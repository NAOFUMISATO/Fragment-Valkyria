
/*****************************************************************//**
 * \file   Stage.cpp
 * \brief  ステージ
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Stage.h"
#include "ModelComponent.h"
#include "ObjectServer.h"
#include "StageModelComponent.h"

using namespace FragmentValkyria::Stage;

Stage::Stage(Game::GameMain& game) : Object::ObjectBase{ game } {
   // スカイスフィアのモデル
   _skySphere = std::make_unique<Model::ModelComponent>(*this);
   _skySphere->SetModel("SkySphere");
   _skySphere->SetScale({ 80.f,  80.f, 80.f });

   _stage = std::make_unique<StageModelComponent>(*this);
   _stage->SetModels("Stage");
   _stage->PixelLightingON();

   auto [charaCollHandle, charaCollNum] = _stage->GetHandleAndCollNum("stage_character_c");
   auto [bossCollHandle, bossCollNum] = _stage->GetHandleAndCollNum("stage_boss_c");
   auto [objCollHandle, objCollNum] = _stage->GetHandleAndCollNum("stage_object_c");

   // プレイヤーのコリジョンフレームをナビメッシュとして使用
   MV1SetupCollInfo(charaCollHandle, charaCollNum, 8, 1, 8);

#ifndef _DEBUG
   MV1SetFrameVisible(charaCollHandle, charaCollNum, false);
   MV1SetFrameVisible(bossCollHandle, bossCollNum, false);
   MV1SetFrameVisible(objCollHandle, objCollNum, false);
#else
   MV1SetFrameOpacityRate(charaCollHandle, charaCollNum,0.6f);
   MV1SetFrameOpacityRate(bossCollHandle, bossCollNum, 1.0f);
   MV1SetFrameOpacityRate(objCollHandle, objCollNum, 0.2f);
#endif
}

void Stage::Update() {
   // スカイスフィアをプレイヤと同じ位置にする
   _skySphere->SetPosition(GetObjServer().GetVecData("PlayerPos"));
}

void Stage::Draw() {
   //_skySphere->Draw(); // スカイスフィア
   _stage->Draw();
}
