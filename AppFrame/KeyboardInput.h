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
         inline bool DownPress() const { return _key & PAD_INPUT_DOWN; }
         /**
          * \brief �L�[�{�[�h�̉����L�[�̃g���K�����Ԃ�
          * \return �����L�[�̉�������
          */
         inline bool DownClick() const { return _trg & PAD_INPUT_DOWN; }
         /**
          * \brief �L�[�{�[�h�̍����L�[�̉��������Ԃ�
          * \return �����L�[�̉�������
          */
         inline bool LeftPress() const { return _key & PAD_INPUT_LEFT; }
         /**
          * \brief �L�[�{�[�h�̍����L�[�̃g���K�����Ԃ�
          * \return �����L�[�̉�������
          */
         inline bool LeftClick() const { return _trg & PAD_INPUT_LEFT; }
         /**
          * \brief �L�[�{�[�h�̉E���L�[�̉��������Ԃ�
          * \return �E���L�[�̉�������
          */
         inline bool RightPress() const { return _key & PAD_INPUT_RIGHT; }
         /**
          * \brief �L�[�{�[�h�̉E���L�[�̃g���K�����Ԃ�
          * \return �E���L�[�̉�������
          */
         inline bool RightClick() const { return _trg & PAD_INPUT_RIGHT; }
         /**
          * \brief �L�[�{�[�h�̏���L�[�̉��������Ԃ�
          * \return ����L�[�̉�������
          */
         inline bool UpPress() const { return _key & PAD_INPUT_UP; }
         /**
          * \brief �L�[�{�[�h�̏���L�[�̃g���K�����Ԃ�
          * \return ����L�[�̉�������
          */
         inline bool UpClick() const { return _trg & PAD_INPUT_UP; }
         /**
          * \brief �L�[�{�[�h��Z�L�[�̃g���K�����Ԃ�
          * \return Z�L�[�̃g���K����
          */
         inline bool ZClick() const { return _trg & PAD_INPUT_1; }
         /**
          * \brief �L�[�{�[�h��X�L�[�̃g���K�����Ԃ�
          * \return X�L�[�̃g���K����
          */
         inline bool XClick() const { return _trg & PAD_INPUT_2; }
         /**
          * \brief �L�[�{�[�h��C�L�[�̃g���K�����Ԃ�
          * \return C�L�[�̃g���K����
          */
         inline bool CClick() const { return _trg & PAD_INPUT_3; }
         /**
          * \brief �L�[�{�[�h��A�L�[�̉��������Ԃ�
          * \return A�L�[�̉�������
          */
         inline bool APress() const { return _key & PAD_INPUT_4; }
         /**
          * \brief �L�[�{�[�h��S�L�[�̉��������Ԃ�
          * \return S�L�[�̉�������
          */
         inline bool SPress() const { return _key & PAD_INPUT_5; }
         /**
          * \brief �L�[�{�[�h��D�L�[�̉��������Ԃ�
          * \return D�L�[�̉�������
          */
         inline bool DPress() const { return _key & PAD_INPUT_6; }
         /**
          * \brief �L�[�{�[�h��Q�L�[�̃g���K�����Ԃ�
          * \return Q�L�[�̃g���K����
          */
         inline bool QClick() const { return _trg & PAD_INPUT_7; }
         /**
          * \brief �L�[�{�[�h��W�L�[�̉��������Ԃ�
          * \return W�L�[�̉�������
          */
         inline bool WPress() const { return _key & PAD_INPUT_8; }
         /**
          * \brief �L�[�{�[�h��ESC�L�[�̃g���K�����Ԃ�
          * \return ESC�L�[�̃g���K����
          */
         inline bool EscClick() const { return _trg & PAD_INPUT_9; }
         /**
          * \brief �L�[�{�[�h��Space�L�[�̃g���K�����Ԃ�
          * \return Space�L�[�̃g���K����
          */
         inline bool SpaceClick() const { return _trg & PAD_INPUT_10; }

      private:
         int _key = 0;   //!< ��������
         int _trg = 0;   //!< �g���K����
      };
   }
}