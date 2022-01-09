
/*****************************************************************//**
 * \file   Stage.cpp
 * \brief  �X�e�[�W
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Stage.h"
#include "ModelComponent.h"
#include "ObjectServer.h"
using namespace FragmentValkyria::Stage;

Stage::Stage(Game::GameMain& game) : Object::ObjectBase{ game } {
   // �X�J�C�X�t�B�A�̃��f��
   _skySphere = std::make_unique<Model::ModelComponent>(*this);
   _skySphere->SetModel("SkySphere");
   _skySphere->SetScale({ 80.f,  80.f, 80.f });
   // �n�ʂ̃��f��
   _ground = std::make_unique<Model::ModelComponent>(*this);
   _ground->SetModel("Ground");
   _ground->SetScale({ 10.f, 10.f, 10.f });

   // �t�H�O�̐ݒ�
   SetFogEnable(TRUE);
   SetFogColor(247, 188, 101);
   SetFogStartEnd(50.0f, 15000.0f);
}

void Stage::Update() {
   // �X�J�C�X�t�B�A���v���C���Ɠ����ʒu�ɂ���
   _skySphere->SetPosition(GetObjServer().GetPosition("PlayerPos"));
}

void Stage::Draw() {
   _skySphere->Draw(); // �X�J�C�X�t�B�A
   _ground->Draw();    // �n��
}
