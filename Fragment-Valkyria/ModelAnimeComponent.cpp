
/*****************************************************************//**
 * \file   ModelAnimeComponent.cpp
 * \brief  ���f���̃A�j���[�V�����Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelAnimeComponent.h"
using namespace FragmentValkyria::Model;

ModelAnimeComponent::ModelAnimeComponent(Object::ObjectBase& owner) : ModelComponent{ owner } {

}

ModelAnimeComponent::~ModelAnimeComponent() {
   MV1DetachAnim(_modelHandle, _attachIndex);
}

void ModelAnimeComponent::Init() {
}

void ModelAnimeComponent::Register(std::string_view key, int animIndex) {
   if (_animeRegistry.contains(key.data())) {
      _animeRegistry.erase(key.data());
   }
   _animeRegistry.emplace(key, animIndex);
}

void ModelAnimeComponent::Update() {
   // �Đ����Ԃ��Z�b�g����
   MV1SetAttachAnimTime(_modelHandle, _attachIndex, _playTime);

   // ���[���h�s��X�V
   ModelComponent::Update();

   // �Đ����Ԃ�i�߂�
   _playTime += (1.f * _timeRate);

   if (_playTime >= _totalTime) {
      if (_repeate) {
         // ���s�[�g�Đ�����
         _playTime = 0.0f;
         ++_repeatedCount;
      }
      else {
         // ���s�[�g�Đ��Ȃ�
         _repeatedCount = 1;
         _playTime = _totalTime;
      }
   }
}

void ModelAnimeComponent::Draw() {
   MV1DrawModel(_modelHandle);
}

void ModelAnimeComponent::ChangeAnime(std::string_view key, bool repeate) {
   auto newAnimIndex = _animeRegistry[key.data()];
   if (_animIndex == newAnimIndex) {
      return;
   }
   _animIndex = newAnimIndex;
   // �A�j���[�V�������f�^�b�`����
   MV1DetachAnim(_modelHandle, _attachIndex);
   // �V�����A�j���[�V�������A�^�b�`����
   _attachIndex = MV1AttachAnim(_modelHandle, newAnimIndex, -1, FALSE);
   // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
   _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachIndex);

   _playTime = 0.0f;
   _repeatedCount = 0;
   _repeate = repeate;
}
