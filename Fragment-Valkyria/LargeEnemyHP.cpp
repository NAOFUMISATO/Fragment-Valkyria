
/*****************************************************************//**
 * \file   LargeEnemyHP.cpp
 * \brief  ���[�W�G�l�~�[�̗͕̑`��N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "LargeEnemyHP.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamLargeEnemyUI.h"

namespace {
   constexpr auto OffSetLeft = 50;   //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetTop = 57;    //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetRight = 948;//!< �I�t�Z�b�g�ʒu�E
   constexpr auto OffSetBottom = 87; //!< �I�t�Z�b�g�ʒu��
   constexpr auto MaxRate = 1.0;     //!< �o�[�����l�̍ő�l

}

using namespace FragmentValkyria::Enemy;

LargeEnemyHP::LargeEnemyHP() {
   _param = std::make_unique<Param::ParamLargeEnemyUI>("bossui");
}

void LargeEnemyHP::Init() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return static_cast<unsigned char>(_param->GetIntParam(paramName));
   };
   _hp = _param->GetDoubleParam("max_hp");
   // �摜�n���h����ResourceServer����擾����
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("BossHP");
   // �I�t�Z�b�g�ʒu������
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };
   // 1�t���[���O�̑O�ʃo�[�̃v���C���[HP���I�t�Z�b�g�E���W�ŏ�����
   _oldFrontHP = OffSetRight;
   // �O�ʃo�[�J���[��������
   _frontColor = { _IntParam("frontcolor_red"),
      _IntParam("frontcolor_green"),_IntParam("frontcolor_blue") };
   // �w�ʃo�[�J���[��������
   _backColor = { _IntParam("backcolor_red"),
      _IntParam("backcolor_green"),_IntParam("backcolor_blue") };
   // �o�[�t���[���̏�����
   _position = _param->GetVecParam("hp_pos");
}

void LargeEnemyHP::Update() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   using Utility = AppFrame::Math::Utility;
   // �Q�[���̃t���[���J�E���g��ModeServer����擾
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto count = modeServer.frameCount();
   // �{�XHP��ObjectServer����擾
   auto& gameInstance = Game::Game::GetInstance();
   _hp = gameInstance.objServer().GetDoubleData("LargeEnemyHP");
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // ���݂̑O��HP�o�[�E���W����`��ԂŌv�Z
   auto frontHP = std::lerp(right, (right - left) * _hp / _DoubleParam("max_hp") + left, MaxRate);
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
      _rate += _DoubleParam("redbar_speed");
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

void LargeEnemyHP::Draw() {
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

