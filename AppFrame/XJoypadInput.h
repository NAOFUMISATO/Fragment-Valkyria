#pragma once
/*****************************************************************//**
 * \file   XJoypadInput.h
 * \brief  XInput�W���C�p�b�h�̓��͊Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <DxLib.h>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���͊Ǘ����O���
    */
   namespace Input {
      /**
       * \class XInput�W���C�p�b�h�̓��͊Ǘ��N���X
       * \brief DxLib::GetJoypadXInputState�œ��͂��󂯎��,�Ǘ�����
       */
      class XJoypadInput {
      public:
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[��̉��������Ԃ�
          * \return �\���L�[��̉�������
          */
         inline bool DUpPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_UP]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[���̉��������Ԃ�
          * \return �\���L�[���̉�������
          */
         inline bool DDownPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_DOWN]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[���̉��������Ԃ�
          * \return �\���L�[���̉�������
          */
         inline bool DLeftPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_LEFT]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[�E�̉��������Ԃ�
          * \return �\���L�[�E�̉�������
          */
         inline bool DRightPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_RIGHT]; }
         /**
          * \brief XInput�W���C�p�b�h��START�{�^���̃g���K�����Ԃ�
          * \return START�{�^���̃g���K����
          */
         inline bool StartClick() const { return _trg.Buttons[XINPUT_BUTTON_START]; }
         /**
          * \brief XInput�W���C�p�b�h��BACK�{�^���̃g���K�����Ԃ�
          * \return BACK�{�^���̃g���K����
          */
         inline bool BackClick() const { return _trg.Buttons[XINPUT_BUTTON_BACK]; }
         /**
          * \brief XInput�W���C�p�b�h�̍��X�e�B�b�N�������݂̃g���K�����Ԃ�
          * \return ���X�e�B�b�N�������݂̃g���K����
          */
         inline bool LeftStickClick() const { return _trg.Buttons[XINPUT_BUTTON_LEFT_THUMB]; }
         /**
          * \brief XInput�W���C�p�b�h�̉E�X�e�B�b�N�������݂̃g���K�����Ԃ�
          * \return �E�X�e�B�b�N�������݂̃g���K����
          */
         inline bool RightStickClick() const { return _trg.Buttons[XINPUT_BUTTON_RIGHT_THUMB]; }
         /**
          * \brief LB�{�^���̃g���K�����Ԃ�
          * \return LB�{�^���̃g���K����
          */
         inline bool LBClick() const { return _trg.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]; }
         /**
          * \brief RB�{�^���̃g���K�����Ԃ�
          * \return RB�{�^���̃g���K����
          */
         inline bool RBClick() const { return _trg.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]; }
         /**
          * \brief A�{�^���̃g���K�����Ԃ�
          * \return A�{�^���̃g���K����
          */
         inline bool AClick() const { return _trg.Buttons[XINPUT_BUTTON_A]; }
         /**
          * \brief B�{�^���̃g���K�����Ԃ�
          * \return B�{�^���̃g���K����
          */
         inline bool BClick() const { return _trg.Buttons[XINPUT_BUTTON_B]; }
         /**
          * \brief X�{�^���̃g���K�����Ԃ�
          * \return X�{�^���̃g���K����
          */
         inline bool XClick() const { return _trg.Buttons[XINPUT_BUTTON_X]; }
         /**
          * \brief Y�{�^���̃g���K�����Ԃ�
          * \return Y�{�^���̃g���K����
          */
         inline bool YClick() const { return _trg.Buttons[XINPUT_BUTTON_Y]; }
         /**
          * \brief ���X�e�B�b�N��X�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return ���X�e�B�b�N��X�����͗�
          */
         inline short LeftStickX() const { return _key.ThumbLX; }
         /**
          * \brief ���X�e�B�b�N��Y�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return ���X�e�B�b�N��Y�����͗�
          */
         inline short LeftStickY() const { return _key.ThumbLY; }
         /**
          * \brief �E�X�e�B�b�N��X�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return ���X�e�B�b�N��X�����͗�
          */
         inline short RightStickX() const { return _key.ThumbRX; }
         /**
          * \brief �E�X�e�B�b�N��Y�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return �E�X�e�B�b�N��Y�����͗�
          */
         inline short RightStickY() const { return _key.ThumbRY; }
         /**
          * \brief ���g���K�[�̓��͗ʂ�Ԃ�(0 �` 255)
          * \return ���g���K�[�̓��͗�
          */
         inline unsigned char LeftTrigger() const { return _key.LeftTrigger; }
         /**
          * \brief �E�g���K�[�̓��͗ʂ�Ԃ�(0 �` 255)
          * \return �E�g���K�[�̓��͗�
          */
         inline unsigned char RightTrigger() const { return _key.RightTrigger; }

      private:
         XINPUT_STATE _key{ 0 };   //!< XInput�W���C�p�b�h�̉�������
         XINPUT_STATE _trg{ 0 };   //!< XInput�W���C�p�b�h�̃g���K����
      };
   }
}
