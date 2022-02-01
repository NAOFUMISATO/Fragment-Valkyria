
/*****************************************************************//**
 * \file   Stage.cpp
 * \brief  �X�e�[�W
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
   // �X�J�C�X�t�B�A�̃��f��
   _skySphere = std::make_unique<Model::ModelComponent>(*this);
   _skySphere->SetModel("SkySphere");
   _skySphere->SetScale({ 80.f,  80.f, 80.f });

   _stage = std::make_unique<StageModelComponent>(*this);
   _stage->SetModels("Boss");
   _stage->PixelLightingON();
}

void BossStage::Update() {
   // �X�J�C�X�t�B�A���v���C���Ɠ����ʒu�ɂ���
   _skySphere->SetPosition(GetObjServer().GetVecData("PlayerPos"));
}

void BossStage::Draw() {
   _skySphere->Draw(); // �X�J�C�X�t�B�A
   _stage->Draw();
}
