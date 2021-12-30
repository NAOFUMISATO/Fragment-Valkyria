#pragma once
/*****************************************************************//**
 * \file   MouseInput.h
 * \brief  マウスの入力管理クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <DxLib.h>
#include "Vector4.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 入力関係
    */
   namespace Input {
      /**
       * \class マウスの入力管理クラス
       * \brief DxLib::GetMouseInput,GetMousePointで入力を受け取り、管理する
       */
      class MouseInput {
      public:
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 左ボタンクリック判定を返す
          * \return 押されたならTRUEを返す
          */
         inline bool LeftClick() { return _trg & MOUSE_INPUT_LEFT; }
         /**
          * \brief 右ボタンクリック判定を返す
          * \return 押されたならTRUEを返す
          */
         inline bool RightClick() { return _trg & MOUSE_INPUT_RIGHT; }
         /**
          * \brief 左ボタン押下判定を返す
          * \return 押されているならTRUEを返す
          */
         inline bool LeftPress() { return _key & MOUSE_INPUT_LEFT; }
         /**
          * \brief 右ボタン押下判定を返す
          * \return 押されているならTRUEを返す
          */
         inline bool RightPress() { return _key & MOUSE_INPUT_RIGHT; }
         /**
          * \brief マウスポインターの座標の取得
          * \return マウスポインターの座標
          */
         inline Math::Vector4 GetPosition() { return _position; }
         /**
          * \brief 座標の変化量の取得
          * \return 座標の変化量
          */
         inline Math::Vector4 GetDelta() { return _delta; }

      private:
         int _key{ 0 };   //!< 押下判定
         int _trg{ 0 };   //!< トリガ判定
         Math::Vector4 _position{ 0,0,0 };  //!< マウスポインター座標
         Math::Vector4 _delta{ 0,0,0 };     //!< マウスポインター座標の変化量
      };
   }
}