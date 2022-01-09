
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
using namespace FragmentValkyria::Stage;

Stage::Stage(Game::GameMain& game) : Object::ObjectBase{ game } {
   // スカイスフィアのモデル
   _skySphere = std::make_unique<Model::ModelComponent>(*this);
   _skySphere->SetModel("SkySphere");
   _skySphere->SetScale({ 80.f,  80.f, 80.f });
   // 地面のモデル
   _ground = std::make_unique<Model::ModelComponent>(*this);
   _ground->SetModel("Ground");
   _ground->SetScale({ 10.f, 10.f, 10.f });

   // フォグの設定
   SetFogEnable(TRUE);
   SetFogColor(247, 188, 101);
   SetFogStartEnd(50.0f, 15000.0f);
}

void Stage::Update() {
   // スカイスフィアをプレイヤと同じ位置にする
   _skySphere->SetPosition(GetObjServer().GetPosition("PlayerPos"));
}

void Stage::Draw() {
   _skySphere->Draw(); // スカイスフィア
   _ground->Draw();    // 地面
}
