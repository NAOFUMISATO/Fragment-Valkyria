#pragma once
/*****************************************************************//**
 * \file   InputManager.cpp
 * \brief  �S�Ă̓��͂̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "InputManager.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���͊֌W
    */
   namespace Input {

      void KeyboardInput::Update() {
         auto keyOld = _key;
         _key = GetJoypadInputState(DX_INPUT_KEY);
         _trg = _key ^ keyOld & _key;
      }

      void MouseInput::Update() {
         auto keyOld = _key;
         _key = GetMouseInput();
         _trg = _key ^ keyOld & _key;

         int mx, my;
         GetMousePoint(&mx, &my);
         Math::Vector4 newPos = { static_cast<float>(mx), static_cast<float>(my), 0 };
         _delta = newPos - _position;
         _position = newPos;
      }

      void XJoypadInput::Update() {
         auto keyold = _key;
         //XInput�W���C�p�b�h�̐��̏��𓾂�
         GetJoypadXInputState(DX_INPUT_PAD1, &_key);
         //XInput�̃{�^���̔z��̗v�f���𓾂�
         auto buttonSize = sizeof(_key.Buttons) / sizeof(_key.Buttons[0]);
         //�S�Ẵ{�^���̃g���K���͂𓾂�
         for (auto i = 0; i < buttonSize; ++i) {
            _trg.Buttons[i] = _key.Buttons[i] ^ keyold.Buttons[i] & _key.Buttons[i];
         }
      }

      void InputManager::Update() {
         _keyboardInput.Update();
         _mouseInput.Update();
         _xJoypadInput.Update();
      }
   }
}