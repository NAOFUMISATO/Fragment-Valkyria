
/*****************************************************************//**
 * \file   PlayerHP.cpp
 * \brief  �v���C���[HP�̕`����s��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "PlayerHP.h"
#include "GameMain.h"

namespace {
   // json�t�@�C������l���擾����
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", { "frontcolor_red" ,
      "frontcolor_green" ,"frontcolor_blue", "backcolor_red" ,"backcolor_green",
      "backcolor_blue" ,"shake_frame","shake_width","redbar_speed","max_hp" });
   const unsigned char FrontColorRed = paramMap["frontcolor_red"];     //!< �O�ʃo�[�̏����J���[�Ԓl
   const unsigned char FrontColorGreen = paramMap["frontcolor_green"]; //!< �O�ʃo�[�̏����J���[�Βl
   const unsigned char FrontColorBlue = paramMap["frontcolor_blue"];   //!< �O�ʃo�[�̏����J���[�l
   const unsigned char BackColorRed = paramMap["backcolor_red"];       //!< �w�ʃo�[�̏����J���[�Ԓl
   const unsigned char BackColorGreen = paramMap["backcolor_green"];   //!< �w�ʃo�[�̏����J���[�Βl
   const unsigned char BackColorBlue = paramMap["backcolor_blue"];     //!< �w�ʃo�[�̏����J���[�l
   const unsigned char ShakeFrame= paramMap["shake_frame"];            //!< �U���t���[��
   const double ShakeWidth = paramMap["shake_width"];                  //!< �U���̑傫��
   const double RedBarSpeed = paramMap["redbar_speed"];                //!< �w�ʃo�[�̌������x
   const double MaxHp= paramMap["max_hp"];                             //!< �v���C���[�ő�HP
   // json�t�@�C������Vector4�̒l���擾����
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "hp_pos" });
   const auto DefalutPos = vecParamMap["hp_pos"];                      //!< �o�[�t���[���ʒu(������W)

   constexpr auto OffSetLeft = 18;   //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetTop = 27;    //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetRight = 327; //!< �I�t�Z�b�g�ʒu�E
   constexpr auto OffSetBottom = 84; //!< �I�t�Z�b�g�ʒu��
   constexpr auto MaxRate = 1.0;     //!< �o�[�����l�̍ő�l

}

using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void PlayerHP::Init() {
   // �摜�n���h����ResourceServer����擾����
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };    // �I�t�Z�b�g�ʒu������
   _oldFrontHP = OffSetRight;                                      // 1�t���[���O�̑O�ʃo�[�̃v���C���[HP���I�t�Z�b�g�E���W�ŏ�����
   _frontColor = { FrontColorRed,FrontColorGreen,FrontColorBlue }; // �O�ʃo�[�J���[��������
   _backColor = { BackColorRed,BackColorGreen,BackColorBlue };     // �w�ʃo�[�J���[��������
   _position = DefalutPos;                                         // �o�[�t���[���̏�����
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   // �Q�[���̃t���[���J�E���g��ModeServer����擾
   auto count = _gameMain.modeServer().frameCount();
   // HP�o�[�U���̏���
   BarShake(count);
   // �v���C���[HP��ObjectServer����擾
   _hp = _gameMain.objServer().GetDoubleData("PlayerHP");
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // ���݂̑O��HP�o�[�E���W����`��ԂŌv�Z
   auto frontHP = std::lerp(right, (right - left) * _hp / MaxHp + left, MaxRate);
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
      _rate += RedBarSpeed;
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
   // �O�ʃo�[�̎O�p�`���W���X�V
   _frontTriangle.SetTriangleParams(
      std::make_pair(x + frontHP, y + top),
      std::make_pair(x + frontHP, y + bottom),
      std::make_pair(x + frontHP + (y + bottom - (y + top)), y + top)
   );
   // �w�ʃo�[�̋�`���W���X�V
   _backBar.SetRectParams(
      x + left,
      y + top,
      x + backHP,
      y + bottom
   );
   // �w�ʃo�[�̎O�p�`���W���X�V
   _backTriangle.SetTriangleParams(
      std::make_pair(x + backHP, y + top),
      std::make_pair(x + backHP, y + bottom),
      std::make_pair(x + backHP + (y + bottom - (y + top)), y + top)
   );
}

void PlayerHP::Draw() {
   // HP���ȉ��Ȃ��HP�o�[�͑S�ĕ`�悵�Ȃ�
   if (_hp > 0){
      auto [backLeft, backTop, backRight, backBottom] = _backBar.GetRectParams();
      // �w�ʃo�[��`�̕`��
      DrawBox(static_cast<int>(backLeft), static_cast<int>(backTop),
         static_cast<int>(backRight), static_cast<int>(backBottom), _backColor.GetColorCode(), TRUE);
      auto [backFirstApex, backSecondApex, backThirdApex] = _backTriangle.GetTriangleParams();
      auto [backFirstX, backFirstY] = backFirstApex;
      auto [backSecondX, backSecondY] = backSecondApex;
      auto [backThirdX, backThirdY] = backThirdApex;
      // �w�ʃo�[�O�p�`�̕`��
      DrawTriangle(static_cast<int>(backFirstX), static_cast<int>(backFirstY), static_cast<int>(backSecondX),
         static_cast<int>(backSecondY), static_cast<int>(backThirdX), static_cast<int>(backThirdY), _backColor.GetColorCode(), TRUE);

      auto [frontLeft, frontTop, frontRight, frontBottom] = _frontBar.GetRectParams();
      // �O�ʃo�[��`�̕`��
      DrawBox(static_cast<int>(frontLeft), static_cast<int>(frontTop),
         static_cast<int>(frontRight), static_cast<int>(frontBottom), _frontColor.GetColorCode(), TRUE);
      auto [frontFirstApex, frontSecondApex, frontThirdApex] = _frontTriangle.GetTriangleParams();
      auto [frontFirstX, frontFirstY] = frontFirstApex;
      auto [frontSecondX, frontSecondY] = frontSecondApex;
      auto [frontThirdX, frontThirdY] = frontThirdApex;
      // �O�ʃo�[�O�p�`�̕`��
      DrawTriangle(static_cast<int>(frontFirstX), static_cast<int>(frontFirstY), static_cast<int>(frontSecondX),
         static_cast<int>(frontSecondY), static_cast<int>(frontThirdX), static_cast<int>(frontThirdY), _frontColor.GetColorCode(), TRUE);
   }
   // �o�[�t���[���̕`��
   SpriteBase::Draw();
}

void PlayerHP::BarShake(unsigned int count) {
   // HP�o�[�U���̏���
   if (_shake) {
      // �U�����n�߂Ă���w��t���[���ȓ��̏���
      if (count - _shakeCnt <= ShakeFrame) {
         auto [posX, posY] = DefalutPos.GetVec2();
         // Utility�N���X���瓙�m�����z�̗������w��͈͓��Ŏ擾
         auto rand = AppFrame::Math::Utility::GetRandom(posX - ShakeWidth, posY + ShakeWidth);
         // �o�[�t���[���̍��W�𗐐��ōX�V
         _position.SetVec2(rand, rand);
      }
      // �U�����n�߂Ă���w��t���[���ȏ�̏���
      else {
         _position = DefalutPos; // �ʒu�̃��Z�b�g
         _shake = false;         // �U���t���OOFF
      }
   }
}
