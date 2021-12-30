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
         inline bool DownPress() { return _key & PAD_INPUT_DOWN; }
         /**
          * \brief �L�[�{�[�h�̍����L�[�̉��������Ԃ�
          * \return �����L�[�̉�������
          */
         inline bool LeftPress() { return _key & PAD_INPUT_LEFT; }
         /**
          * \brief �L�[�{�[�h�̉E���L�[�̉��������Ԃ�
          * \return �E���L�[�̉�������
          */
         inline bool RightPress() { return _key & PAD_INPUT_RIGHT; }
         /**
          * \brief �L�[�{�[�h�̏���L�[�̉��������Ԃ�
          * \return ����L�[�̉�������
          */
         inline bool UpPress() { return _key & PAD_INPUT_UP; }
         /**
          * \brief �L�[�{�[�h��Z�L�[�̃g���K�����Ԃ�
          * \return Z�L�[�̃g���K����
          */
         inline bool ZClick() { return _trg & PAD_INPUT_1; }
         /**
          * \brief �L�[�{�[�h��X�L�[�̃g���K�����Ԃ�
          * \return X�L�[�̃g���K����
          */
         inline bool XClick() { return _trg & PAD_INPUT_2; }
         /**
          * \brief �L�[�{�[�h��C�L�[�̃g���K�����Ԃ�
          * \return C�L�[�̃g���K����
          */
         inline bool CClick() { return _trg & PAD_INPUT_3; }
         /**
          * \brief �L�[�{�[�h��A�L�[�̉��������Ԃ�
          * \return A�L�[�̉�������
          */
         inline bool APress() { return _key & PAD_INPUT_4; }
         /**
          * \brief �L�[�{�[�h��S�L�[�̉��������Ԃ�
          * \return S�L�[�̉�������
          */
         inline bool SPress() { return _key & PAD_INPUT_5; }
         /**
          * \brief �L�[�{�[�h��D�L�[�̉��������Ԃ�
          * \return D�L�[�̉�������
          */
         inline bool DPress() { return _key & PAD_INPUT_6; }
         /**
          * \brief �L�[�{�[�h��Q�L�[�̃g���K�����Ԃ�
          * \return Q�L�[�̃g���K����
          */
         inline bool QClick() { return _trg & PAD_INPUT_7; }
         /**
          * \brief �L�[�{�[�h��W�L�[�̉��������Ԃ�
          * \return W�L�[�̉�������
          */
         inline bool WPress() { return _key & PAD_INPUT_8; }
         /**
          * \brief �L�[�{�[�h��ESC�L�[�̃g���K�����Ԃ�
          * \return ESC�L�[�̃g���K����
          */
         inline bool EscClick() { return _trg & PAD_INPUT_9; }
         /**
          * \brief �L�[�{�[�h��Space�L�[�̃g���K�����Ԃ�
          * \return Space�L�[�̃g���K����
          */
         inline bool SpaceClick() { return _trg & PAD_INPUT_10; }

      private:
         int _key = 0;   //!< ��������
         int _trg = 0;   //!< �g���K����
      };
   }
}