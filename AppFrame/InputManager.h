#pragma once
/*****************************************************************//**
 * \file   InputManager.h
 * \brief  �S�Ă̓��͂̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "XJoypadInput.h"
#include <memory>

 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���͊֌W
    */
   namespace Input {
      /**
       * \class �S�Ă̓��͂̈ꊇ�Ǘ��N���X
       * \brief �e�f�o�C�X�̓��͊Ǘ��N���X�̎Q�Ƃ��擾���A�ꊇ�Ǘ�����
       */
      class InputManager {
      public:
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �L�[�{�[�h�̓��͊Ǘ��N���X�̎Q�Ƃ��擾
          * \return �L�[�{�[�h�̓��͊Ǘ��N���X�̎Q��
          */
         KeyboardInput GetKeyboard() { return _keyboardInput; }
         /**
          * \brief �}�E�X�̓��͊Ǘ��N���X�̎Q�Ƃ��擾
          * \return �}�E�X�̓��͊Ǘ��N���X�̎Q��
          */
         MouseInput GetMouse() { return _mouseInput; }
         /**
          * \brief XInput�W���C�p�b�h�̓��͊Ǘ��N���X�̎Q�Ƃ��擾
          * \return XInput�W���C�p�b�h�̓��͊Ǘ��N���X�̎Q��
          */
         XJoypadInput GetXJoypad() { return _xJoypadInput; }

      private:
         KeyboardInput _keyboardInput; //!< �L�[�{�[�h�̓��͊Ǘ��N���X
         MouseInput _mouseInput;       //!< �}�E�X�̓��͊Ǘ��N���X
         XJoypadInput _xJoypadInput;   //!< XInput�W���C�p�b�h�̓��͊Ǘ��N���X
      };
   }
}