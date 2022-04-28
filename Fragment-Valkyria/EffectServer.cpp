
/*****************************************************************//**
 * \file   EffectServer.cpp
 * \brief  �G�t�F�N�g�T�[�o�[
 *
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "EffectServer.h"
#include <EffekseerForDxLib.h>

using namespace FragmentValkyria::Effect;

void EffectServer::Add(std::unique_ptr<EffectBase> efc) {
   efc->Init();
   if (_updating) {
      // �X�V���͕ۗ����̓��I�z��ɒǉ�����
      _pendingEffects.emplace_back(std::move(efc));
   }
   else {
      //�X�V���łȂ���Ώ������񂷗p�̓��I�z��ɒǉ�����
      _runEffects.emplace_back(std::move(efc));
   }
}

void EffectServer::Input(AppFrame::Input::InputManager& input) {
   _updating = true;
   // _runEffects�ɓo�^����Ă���Active��Ԃ̃G�t�F�N�g�̓��͏�������
   for (auto&& efc : _runEffects) {
      if (efc->IsActive()) {
         efc->Input(input);
      }
   }
   _updating = false;
}

void EffectServer::Update() {
   _updating = true;
   // _runEffects�ɓo�^����Ă���Active��Ԃ̃G�t�F�N�g�̍X�V��������
   for (auto&& efc : _runEffects) {
      if (efc->IsActive()) {
         efc->Update();
      }
   }
   Effekseer_Sync3DSetting();
   UpdateEffekseer3D();
   _updating = false;

   // �ۗ����̃G�t�F�N�g��_runEffects�Ɉړ�����
   _runEffects.insert(_runEffects.end(),
      make_move_iterator(_pendingEffects.begin()),
      make_move_iterator(_pendingEffects.end()));

   // �ۗ����̃G�t�F�N�g���폜����
   _pendingEffects.clear();

   // ���񂾃G�t�F�N�g���폜����
   erase_if(_runEffects, [](auto&& act) { return act->IsDead(); });
}

void EffectServer::Render() {
   for (auto&& efc : _runEffects) {
      // _runEffects�ɓo�^����Ă���Active��Ԃ̃G�t�F�N�g�̕`�揈������
      if (efc->IsActive()) {
         efc->Draw();
      }
   }
   DrawEffekseer3D();
}

void EffectServer::Clear() {
   // �S�Ă̓��I�z����̗v�f���폜����
   for (auto& effect : _runEffects) {
      effect->StopEffect();
   }
   _runEffects.clear();
   _pendingEffects.clear();
}