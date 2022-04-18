
/*****************************************************************//**
 * \file   PlayerHP.cpp
 * \brief  �v���C���[HP�̕`����s��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "PlayerHP.h"
#include "GameMain.h"
#include "ObjectServer.h"
#include "ParamPlayerUI.h"

namespace {
   constexpr auto OffSetLeft = 5;   //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetTop = 7;    //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetRight = 457; //!< �I�t�Z�b�g�ʒu�E
   constexpr auto OffSetBottom = 32; //!< �I�t�Z�b�g�ʒu��
   constexpr auto MaxRate = 1.0;     //!< �o�[�����l�̍ő�l

}

using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
   _param = std::make_unique<Param::ParamPlayerUI>(_gameMain, "playerui");
   _playerParam = std::make_unique < Param::ParamPlayer>(_gameMain, "player");
}

void PlayerHP::Init() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return static_cast<unsigned char>(_param->GetIntParam(paramName));
   };
   _hp = _gameMain.playerHp();
   // �摜�n���h����ResourceServer����擾����
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };    // �I�t�Z�b�g�ʒu������
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // 1�t���[���O�̑O�ʃo�[�̃v���C���[HP����`��Ԃɂ����W�����ߏ�����
   _oldFrontHP = std::lerp(right, (right - left) * _hp /
      _playerParam->GetDoubleParam("max_hp") + left, MaxRate);
   _frontColor = { _IntParam("frontcolor_red"),
      _IntParam("frontcolor_green"),_IntParam("frontcolor_blue") }; // �O�ʃo�[�J���[��������
   _backColor = { _IntParam("backcolor_red"),
      _IntParam("backcolor_green"),_IntParam("backcolor_blue") };   // �w�ʃo�[�J���[��������
   _position = _param->GetVecParam("hp_pos");                       // �o�[�t���[���̏�����
   _shake = false;                                                  // �U���t���OOFF
   _rateReset = false;                                              // �w�ʃo�[�����t���O��OFF
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   // �Q�[���̃t���[���J�E���g��ModeServer����擾
   auto count = _gameMain.modeServer().frameCount();
   // HP�o�[�U���̏���
   BarShake(count);
   // �v���C���[HP���擾
   _hp = _gameMain.playerHp();
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // ���݂̑O��HP�o�[�E���W����`��ԂŌv�Z
   auto frontHP = std::lerp(right, (right - left) * _hp / 
      _playerParam->GetDoubleParam("max_hp") + left, MaxRate);
   // HP���������m�����ۂ̏���
   if (frontHP < _oldFrontHP) {
      // �U���t���OON
      _shake = true;
      // �U���t���[���̃��Z�b�g
      _shakeCnt = count;
   }
   // �w�ʃo�[�������̏���
   if (frontHP < _oldBackHP) {
      // ���݂�HP�o�[�E���W��ۑ�
      _oldFrontHP = frontHP;
      // �w�ʃo�[�����̏I������
      if (_rateReset) {
         _rate = 0.0;        // �w�ʃo�[�����l�̃��Z�b�g
         _rateReset = false; // �w�ʃo�[�����t���O��OFF
      }
      // �w�ʃo�[�����l��i�߂�
      _rate += _param->GetDoubleParam("redbar_speed");
   }
   // �w�ʃo�[�E���W���O�ʃo�[�E���W��蓯����W�ȉ��̏���
   else {
      _rateReset = true;   // �w�ʃo�[�����t���O��ON
      _rate = MaxRate;     // �w�ʃo�[�����l���ő�l�ɂ���
   }
   // ���݂̔w��HP�o�[�E���W����`��ԂŌv�Z���A�w��̒l������������
   auto backHP = std::lerp(_oldBackHP, frontHP, _rate);
   // �w�ʃo�[�E���W���O�ʃo�[�E���W��蓯����W�ȉ��̏���
   if (frontHP >= backHP) {
      // ���݂�HP�o�[�E���W��ۑ�
      _oldBackHP = backHP;
   }
   // �O�ʃo�[�̋�`���W���X�V
   auto [x, y] = _position.GetVec2();
   _frontBar.SetRectParams(
      x + left,
      y + top,
      x + frontHP,
      y + bottom
   );
   // �w�ʃo�[�̋�`���W���X�V
   _backBar.SetRectParams(
      x + left,
      y + top,
      x + backHP,
      y + bottom
   );
}

void PlayerHP::Draw() {
   // HP���ȉ��Ȃ��HP�o�[�͑S�ĕ`�悵�Ȃ�
   if (_hp > 0) {
      auto [backLeft, backTop, backRight, backBottom] = _backBar.GetRectParams();
      // �w�ʃo�[��`�̕`��
      DrawBox(static_cast<int>(backLeft), static_cast<int>(backTop),
         static_cast<int>(backRight), static_cast<int>(backBottom), _backColor.GetColorCode(), TRUE);

      auto [frontLeft, frontTop, frontRight, frontBottom] = _frontBar.GetRectParams();
      // �O�ʃo�[��`�̕`��
      DrawBox(static_cast<int>(frontLeft), static_cast<int>(frontTop),
         static_cast<int>(frontRight), static_cast<int>(frontBottom), _frontColor.GetColorCode(), TRUE);
   }
   // �o�[�t���[���̕`��
   SpriteBase::Draw();
}

void PlayerHP::BarShake(unsigned int count) {
   // HP�o�[�U���̏���
   if (_shake) {
      // �U�����n�߂Ă���w��t���[���ȓ��̏���
      const auto ShakeFrame = static_cast<unsigned int>(_param->GetIntParam("shake_frame"));
      const auto DefalutPos = _param->GetVecParam("hp_pos");
      const auto ShakeWidth = _param->GetDoubleParam("shake_width");
      if (count - _shakeCnt <= ShakeFrame) {
         auto [posX, posY] = DefalutPos.GetVec2();
         // Utility�N���X���瓙�m�����z�̗������w��͈͓��Ŏ擾
         auto randX = AppFrame::Math::Utility::GetRandom(posX - ShakeWidth, posX + ShakeWidth);
         auto randY = AppFrame::Math::Utility::GetRandom(posY - ShakeWidth, posY + ShakeWidth);
         // �o�[�t���[���̍��W�𗐐��ōX�V
         _position.SetVec2(randX, randY);
      }
      // �U�����n�߂Ă���w��t���[���ȏ�̏���
      else {
         _position = DefalutPos; // �ʒu�̃��Z�b�g
         _shake = false;         // �U���t���OOFF
      }
   }
}

