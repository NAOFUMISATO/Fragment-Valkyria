#pragma once
/*****************************************************************//**
 * \file   InputManager.cpp
 * \brief  全ての入力の一括管理
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "InputManager.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 入力関係
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
         //XInputジョイパッドの生の情報を得る
         GetJoypadXInputState(DX_INPUT_PAD1, &_key);
         //XInputのボタンの配列の要素数を得る
         auto buttonSize = sizeof(_key.Buttons) / sizeof(_key.Buttons[0]);
         //全てのボタンのトリガ入力を得る
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