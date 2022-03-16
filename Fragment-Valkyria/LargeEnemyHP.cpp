
#include "LargeEnemyHP.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   // json�t�@�C������l���擾����
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("bossui", { "frontcolor_red" ,
      "frontcolor_green" ,"frontcolor_blue", "backcolor_red" ,"backcolor_green",
      "backcolor_blue" ,"redbar_speed","max_hp" });
   const unsigned char FrontColorRed = paramMap["frontcolor_red"];     //!< �O�ʃo�[�̏����J���[�Ԓl
   const unsigned char FrontColorGreen = paramMap["frontcolor_green"]; //!< �O�ʃo�[�̏����J���[�Βl
   const unsigned char FrontColorBlue = paramMap["frontcolor_blue"];   //!< �O�ʃo�[�̏����J���[�l
   const unsigned char BackColorRed = paramMap["backcolor_red"];       //!< �w�ʃo�[�̏����J���[�Ԓl
   const unsigned char BackColorGreen = paramMap["backcolor_green"];   //!< �w�ʃo�[�̏����J���[�Βl
   const unsigned char BackColorBlue = paramMap["backcolor_blue"];     //!< �w�ʃo�[�̏����J���[�l
   const double RedBarSpeed = paramMap["redbar_speed"];                //!< �w�ʃo�[�̌������x
   const double MaxHp = paramMap["max_hp"];                            //!< �{�X�ő�HP
   // json�t�@�C������Vector4�̒l���擾����
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("bossui", { "hp_pos" });
   const auto DefalutPos = vecParamMap["hp_pos"];                      //!< �o�[�t���[���ʒu(������W)

   constexpr auto OffSetLeft = 50;   //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetTop = 57;    //!< �I�t�Z�b�g�ʒu��
   constexpr auto OffSetRight = 948;//!< �I�t�Z�b�g�ʒu�E
   constexpr auto OffSetBottom = 87; //!< �I�t�Z�b�g�ʒu��
   constexpr auto MaxRate = 1.0;     //!< �o�[�����l�̍ő�l

}

using namespace FragmentValkyria::Enemy;

LargeEnemyHP::LargeEnemyHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void LargeEnemyHP::Init() {
   // �摜�n���h����ResourceServer����擾����
   _hp = 1000.0;
   _grHandle = GetResServer().GetTexture("BossHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };    // �I�t�Z�b�g�ʒu������
   _oldFrontHP = OffSetRight;                                      // 1�t���[���O�̑O�ʃo�[�̃v���C���[HP���I�t�Z�b�g�E���W�ŏ�����
   _frontColor = { FrontColorRed,FrontColorGreen,FrontColorBlue }; // �O�ʃo�[�J���[��������
   _backColor = { BackColorRed,BackColorGreen,BackColorBlue };     // �w�ʃo�[�J���[��������
   _position = DefalutPos;                                         // �o�[�t���[���̏�����
}

void LargeEnemyHP::Update() {
   using Utility = AppFrame::Math::Utility;
   // �Q�[���̃t���[���J�E���g��ModeServer����擾
   auto count = _gameMain.modeServer().frameCount();
   // �{�XHP��ObjectServer����擾
   _hp = _gameMain.objServer().GetDoubleData("LargeEnemyHP");
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // ���݂̑O��HP�o�[�E���W����`��ԂŌv�Z
   auto frontHP = std::lerp(right, (right - left) * _hp / MaxHp + left, MaxRate);
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

