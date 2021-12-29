#pragma once
/*****************************************************************//**
 * \file   InputManager.h
 * \brief  全ての入力の一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "XJoypadInput.h"
#include <memory>

 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 入力関係
    */
   namespace Input {
      /**
       * \class 全ての入力の一括管理クラス
       * \brief 各デバイスの入力管理クラスの参照を取得し、一括管理する
       */
      class InputManager {
      public:
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief キーボードの入力管理クラスの参照を取得
          * \return キーボードの入力管理クラスの参照
          */
         KeyboardInput GetKeyboard() { return _keyboardInput; }
         /**
          * \brief マウスの入力管理クラスの参照を取得
          * \return マウスの入力管理クラスの参照
          */
         MouseInput GetMouse() { return _mouseInput; }
         /**
          * \brief XInputジョイパッドの入力管理クラスの参照を取得
          * \return XInputジョイパッドの入力管理クラスの参照
          */
         XJoypadInput GetXJoypad() { return _xJoypadInput; }

      private:
         KeyboardInput _keyboardInput; //!< キーボードの入力管理クラス
         MouseInput _mouseInput;       //!< マウスの入力管理クラス
         XJoypadInput _xJoypadInput;   //!< XInputジョイパッドの入力管理クラス
      };
   }
}