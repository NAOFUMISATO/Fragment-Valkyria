
/*****************************************************************//**
 * \file   ModelAnimeComponent.cpp
 * \brief  ���f���̃A�j���[�V�����Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelAnimeComponent.h"
#include "GameMain.h"
#include "ObjectBase.h"
using namespace FragmentValkyria::Model;

ModelAnimeComponent::ModelAnimeComponent(Object::ObjectBase& owner) : ModelComponent{ owner } {

}

ModelAnimeComponent::~ModelAnimeComponent() {
   MV1DetachAnim(_modelHandle, _attachNum);
}

void ModelAnimeComponent::Init() {
}

void ModelAnimeComponent::Update() {

   // �Đ����Ԃ��Z�b�g����
   MV1SetAttachAnimTime(_modelHandle, _attachNum, _playTime);

   // ���[���h�s��X�V
   ModelComponent::Update();

   // �Đ����Ԃ�i�߂�
   _playTime += (1.f * static_cast<float>(_timeRate));

   // �Đ����Ԃ����Đ����Ԉȏォ
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

   // �A�j���[�V�����u�����h�̏���
   if (_blending) {
      _blendRate += 0.1f;  // �u�����h���̕ω�
      _repeatedCount = 0;  // �u�����h���̓A�j���[�V�����񐔂��㏸�����Ȃ�
      // ���݂̃A�j���[�V�����̃u�����h�������X�Ɍ���������
      MV1SetAttachAnimBlendRate(_modelHandle, _attachNum, 1.f - _blendRate);
      // ���̃A�j���[�V�����̃u�����h�������X�ɏ㏸������
      MV1SetAttachAnimBlendRate(_modelHandle, _nextAttachNum, _blendRate);
      // �u�����h��������ɒB�����Ȃ�A�A�j���[�V�����u�����h�̏I���������s��
      if (_blendRate >= 1.0f) {
         _animIndex = _newAnimIndex;  // �A�j���ԍ��̍X�V
         MV1DetachAnim(_modelHandle, _attachNum);     // ���݂̃A�j���[�V�������f�^�b�`
         MV1DetachAnim(_modelHandle, _nextAttachNum); // ���̃A�j���[�V�������f�^�b�`
         // �V�����A�j���[�V�����ɃA�^�b�`����
         _attachNum = MV1AttachAnim(_modelHandle, _newAnimIndex, -1, FALSE);
         // ���Đ����Ԃ̎擾
         _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachNum);
         _playTime = 0.0f;  // �Đ����Ԃ̏�����
         _blending = false; // �u�����h�����̔����false
      }
   }
}

//void ModelAnimeComponent::Draw() {
//   MV1DrawModel(_modelHandle);
//}

void ModelAnimeComponent::ChangeAnime(std::string_view animeName, bool repeate,double animeSpeed) {
   // �A�j���[�V�����u�����h���ɁAChangeAnime�֐����Ă񂾂Ȃ�u�����h�̏I���������s��
   if (_blending) {
      _animIndex = _newAnimIndex;  // �A�j���ԍ��̍X�V
      MV1DetachAnim(_modelHandle, _attachNum);     // ���݂̃A�j���[�V�������f�^�b�`
      MV1DetachAnim(_modelHandle, _nextAttachNum); // ���̃A�j���[�V�������f�^�b�`
      // �V�����A�j���[�V�����ɃA�^�b�`����
      _attachNum = MV1AttachAnim(_modelHandle, _newAnimIndex, -1, FALSE);
      // ���Đ����Ԃ̎擾
      _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachNum);
      _playTime = 0.0f;  // �Đ����Ԃ̏�����
      _blending = false; // �u�����h�����̔����false
   }
   // �V�����A�^�b�`����A�j���ԍ���ResourceServer����擾����
   _newAnimIndex = _owner.gameMain().resServer().GetModelAnimIndex(_key, animeName);
   // ����̃A�j���[�V�����ɐ؂�ւ��悤�Ƃ����Ȃ�Ԃ�
   if (_animIndex == _newAnimIndex) {
      return;
   }
   // �A�j���[�V�����u�����h�̂��߂ɐV�����A�j���[�V�����ɃA�^�b�`���Ă���
   _nextAttachNum = MV1AttachAnim(_modelHandle, _newAnimIndex,-1,FALSE);
   _blendRate = 0.f;    // �u�����h���̏�����
   _blending = true;    // �u�����h�����̔����true
   _repeate = repeate;  // ��������A�j���[�V�������J��Ԃ����̔�����擾
   _timeRate = animeSpeed;
   _repeatedCount = 0;  // �A�j���[�V�����񐔂̏�����
}
