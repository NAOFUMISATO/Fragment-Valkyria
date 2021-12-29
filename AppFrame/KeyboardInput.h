#pragma once
/*****************************************************************//**
 * \file   KeyboardInput.h
 * \brief  �L�[�{�[�h�̓��͊Ǘ��N���X
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
    * \brief ���͊֌W
    */
   namespace Input {
      /**
       * \class �L�[�{�[�h�̓��͊Ǘ��N���X
       * \brief DxLib::GetJoypadInputState�ŃL�[�{�[�h�̂ݓ��͂��󂯎��A�Ǘ�����
       */
      class KeyboardInput {
      public:
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �L�[�{�[�h�̉����L�[�̉��������Ԃ�
          * \return �����L�[�̉�������
          */
         bool DownPress() { return _key & PAD_INPUT_DOWN; }
         /**
          * \brief �L�[�{�[�h�̍����L�[�̉��������Ԃ�
          * \return �����L�[�̉�������
          */
         bool LeftPress() { return _key & PAD_INPUT_LEFT; }
         /**
          * \brief �L�[�{�[�h�̉E���L�[�̉��������Ԃ�
          * \return �E���L�[�̉�������
          */
         bool RightPress() { return _key & PAD_INPUT_RIGHT; }
         /**
          * \brief �L�[�{�[�h�̏���L�[�̉��������Ԃ�
          * \return ����L�[�̉�������
          */
         bool UpPress() { return _key & PAD_INPUT_UP; }
         /**
          * \brief �L�[�{�[�h��Z�L�[�̃g���K�����Ԃ�
          * \return Z�L�[�̃g���K����
          */
         bool ZClick() { return _trg & PAD_INPUT_1; }
         /**
          * \brief �L�[�{�[�h��X�L�[�̃g���K�����Ԃ�
          * \return X�L�[�̃g���K����
          */
         bool XClick() { return _trg & PAD_INPUT_2; }
         /**
          * \brief �L�[�{�[�h��C�L�[�̃g���K�����Ԃ�
          * \return C�L�[�̃g���K����
          */
         bool CClick() { return _trg & PAD_INPUT_3; }
         /**
          * \brief �L�[�{�[�h��A�L�[�̉��������Ԃ�
          * \return A�L�[�̉�������
          */
         bool APress() { return _key & PAD_INPUT_4; }
         /**
          * \brief �L�[�{�[�h��S�L�[�̉��������Ԃ�
          * \return S�L�[�̉�������
          */
         bool SPress() { return _key & PAD_INPUT_5; }
         /**
          * \brief �L�[�{�[�h��D�L�[�̉��������Ԃ�
          * \return D�L�[�̉�������
          */
         bool DPress() { return _key & PAD_INPUT_6; }
         /**
          * \brief �L�[�{�[�h��Q�L�[�̃g���K�����Ԃ�
          * \return Q�L�[�̃g���K����
          */
         bool QClick() { return _trg & PAD_INPUT_7; }
         /**
          * \brief �L�[�{�[�h��W�L�[�̉��������Ԃ�
          * \return W�L�[�̉�������
          */
         bool WPress() { return _key & PAD_INPUT_8; }
         /**
          * \brief �L�[�{�[�h��ESC�L�[�̃g���K�����Ԃ�
          * \return ESC�L�[�̃g���K����
          */
         bool EscClick() { return _trg & PAD_INPUT_9; }
         /**
          * \brief �L�[�{�[�h��Space�L�[�̃g���K�����Ԃ�
          * \return Space�L�[�̃g���K����
          */
         bool SpaceClick() { return _trg & PAD_INPUT_10; }

      private:
         int _key = 0;   //!< ��������
         int _trg = 0;   //!< �g���K����
      };
   }
}