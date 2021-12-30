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
         bool DUpPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_UP]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[���̉��������Ԃ�
          * \return �\���L�[���̉�������
          */
         bool DDownPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_DOWN]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[���̉��������Ԃ�
          * \return �\���L�[���̉�������
          */
         bool DLeftPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_LEFT]; }
         /**
          * \brief XInput�W���C�p�b�h�̏\���L�[�E�̉��������Ԃ�
          * \return �\���L�[�E�̉�������
          */
         bool DRightPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_RIGHT]; }
         /**
          * \brief XInput�W���C�p�b�h��START�{�^���̃g���K�����Ԃ�
          * \return START�{�^���̃g���K����
          */
         bool StartClick() { return _trg.Buttons[XINPUT_BUTTON_START]; }
         /**
          * \brief XInput�W���C�p�b�h��BACK�{�^���̃g���K�����Ԃ�
          * \return BACK�{�^���̃g���K����
          */
         bool BackClick() { return _trg.Buttons[XINPUT_BUTTON_BACK]; }
         /**
          * \brief XInput�W���C�p�b�h�̍��X�e�B�b�N�������݂̃g���K�����Ԃ�
          * \return ���X�e�B�b�N�������݂̃g���K����
          */
         bool LeftStickClick() { return _trg.Buttons[XINPUT_BUTTON_LEFT_THUMB]; }
         /**
          * \brief XInput�W���C�p�b�h�̉E�X�e�B�b�N�������݂̃g���K�����Ԃ�
          * \return �E�X�e�B�b�N�������݂̃g���K����
          */
         bool RightStickClick() { return _trg.Buttons[XINPUT_BUTTON_RIGHT_THUMB]; }
         /**
          * \brief LB�{�^���̃g���K�����Ԃ�
          * \return LB�{�^���̃g���K����
          */
         bool LBClick() { return _trg.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]; }
         /**
          * \brief RB�{�^���̃g���K�����Ԃ�
          * \return RB�{�^���̃g���K����
          */
         bool RBClick() { return _trg.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]; }
         /**
          * \brief A�{�^���̃g���K�����Ԃ�
          * \return A�{�^���̃g���K����
          */
         bool AClick() { return _trg.Buttons[XINPUT_BUTTON_A]; }
         /**
          * \brief B�{�^���̃g���K�����Ԃ�
          * \return B�{�^���̃g���K����
          */
         bool BClick() { return _trg.Buttons[XINPUT_BUTTON_B]; }
         /**
          * \brief X�{�^���̃g���K�����Ԃ�
          * \return X�{�^���̃g���K����
          */
         bool XClick() { return _trg.Buttons[XINPUT_BUTTON_X]; }
         /**
          * \brief Y�{�^���̃g���K�����Ԃ�
          * \return Y�{�^���̃g���K����
          */
         bool YClick() { return _trg.Buttons[XINPUT_BUTTON_Y]; }
         /**
          * \brief ���X�e�B�b�N��X�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return ���X�e�B�b�N��X�����͗�
          */
         bool LeftStickX() { return _key.ThumbLX; }
         /**
          * \brief ���X�e�B�b�N��Y�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return ���X�e�B�b�N��Y�����͗�
          */
         bool LeftStickY() { return _key.ThumbLY; }
         /**
          * \brief �E�X�e�B�b�N��X�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return �E�X�e�B�b�N��X�����͗�
          */
         bool RightStickX() { return _key.ThumbRX; }
         /**
          * \brief �E�X�e�B�b�N��Y�����͗ʂ�Ԃ�(-32768 �` 32767)
          * \return �E�X�e�B�b�N��Y�����͗�
          */
         bool RightStickY() { return _key.ThumbRY; }
         /**
          * \brief ���g���K�[�̓��͗ʂ�Ԃ�(0 �` 255)
          * \return ���g���K�[�̓��͗�
          */
         bool LeftTrigger() { return _key.LeftTrigger; }
         /**
          * \brief �E�g���K�[�̓��͗ʂ�Ԃ�(0 �` 255)
          * \return �E�g���K�[�̓��͗�
          */
         bool RightTrigger() { return _key.RightTrigger; }

      private:
         XINPUT_STATE _key{ 0 };   //!< XInput�W���C�p�b�h�̉�������
         XINPUT_STATE _trg{ 0 };   //!< XInput�W���C�p�b�h�̃g���K����
      };
   }
}
