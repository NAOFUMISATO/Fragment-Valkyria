
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

Stage::Stage() {
   _stageComponent = std::make_unique<StageModelComponent>(*this);
   _stageComponent->SetModels("Stage");
   _stageComponent->PixelLightingON();

   auto [charaCollHandle, charaCollNum] = _stageComponent->GetHandleAndFrameNum("stage_character_c");
   auto [bossCollHandle, bossCollNum] = _stageComponent->GetHandleAndFrameNum("stage_boss_c");
   auto [objCollHandle, objCollNum] = _stageComponent->GetHandleAndFrameNum("stage_object_c");

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

void Stage::Draw() {
   _stageComponent->Draw();
}
