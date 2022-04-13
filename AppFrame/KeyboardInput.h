#pragma once
/*****************************************************************//**
 * \file   KeyboardInput.h
 * \brief  キーボードの入力管理クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <DxLib.h>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 入力関係
    */
   namespace Input {
      /**
       * \class キーボードの入力管理クラス
       * \brief DxLib::GetJoypadInputStateでキーボードのみ入力を受け取り、管理する
       */
      class KeyboardInput {
      public:
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief キーボードの下矢印キーの押下判定を返す
          * \return 下矢印キーの押下判定
          */
         inline bool DownPress() const { return _key & PAD_INPUT_DOWN; }
         /**
          * \brief キーボードの下矢印キーのトリガ判定を返す
          * \return 下矢印キーの押下判定
          */
         inline bool DownClick() const { return _trg & PAD_INPUT_DOWN; }
         /**
          * \brief キーボードの左矢印キーの押下判定を返す
          * \return 左矢印キーの押下判定
          */
         inline bool LeftPress() const { return _key & PAD_INPUT_LEFT; }
         /**
          * \brief キーボードの左矢印キーのトリガ判定を返す
          * \return 左矢印キーの押下判定
          */
         inline bool LeftClick() const { return _trg & PAD_INPUT_LEFT; }
         /**
          * \brief キーボードの右矢印キーの押下判定を返す
          * \return 右矢印キーの押下判定
          */
         inline bool RightPress() const { return _key & PAD_INPUT_RIGHT; }
         /**
          * \brief キーボードの右矢印キーのトリガ判定を返す
          * \return 右矢印キーの押下判定
          */
         inline bool RightClick() const { return _trg & PAD_INPUT_RIGHT; }
         /**
          * \brief キーボードの上矢印キーの押下判定を返す
          * \return 上矢印キーの押下判定
          */
         inline bool UpPress() const { return _key & PAD_INPUT_UP; }
         /**
          * \brief キーボードの上矢印キーのトリガ判定を返す
          * \return 上矢印キーの押下判定
          */
         inline bool UpClick() const { return _trg & PAD_INPUT_UP; }
         /**
          * \brief キーボードのZキーのトリガ判定を返す
          * \return Zキーのトリガ判定
          */
         inline bool ZClick() const { return _trg & PAD_INPUT_1; }
         /**
          * \brief キーボードのXキーのトリガ判定を返す
          * \return Xキーのトリガ判定
          */
         inline bool XClick() const { return _trg & PAD_INPUT_2; }
         /**
          * \brief キーボードのCキーのトリガ判定を返す
          * \return Cキーのトリガ判定
          */
         inline bool CClick() const { return _trg & PAD_INPUT_3; }
         /**
          * \brief キーボードのAキーの押下判定を返す
          * \return Aキーの押下判定
          */
         inline bool APress() const { return _key & PAD_INPUT_4; }
         /**
          * \brief キーボードのSキーの押下判定を返す
          * \return Sキーの押下判定
          */
         inline bool SPress() const { return _key & PAD_INPUT_5; }
         /**
          * \brief キーボードのDキーの押下判定を返す
          * \return Dキーの押下判定
          */
         inline bool DPress() const { return _key & PAD_INPUT_6; }
         /**
          * \brief キーボードのQキーのトリガ判定を返す
          * \return Qキーのトリガ判定
          */
         inline bool QClick() const { return _trg & PAD_INPUT_7; }
         /**
          * \brief キーボードのWキーの押下判定を返す
          * \return Wキーの押下判定
          */
         inline bool WPress() const { return _key & PAD_INPUT_8; }
         /**
          * \brief キーボードのESCキーのトリガ判定を返す
          * \return ESCキーのトリガ判定
          */
         inline bool EscClick() const { return _trg & PAD_INPUT_9; }
         /**
          * \brief キーボードのSpaceキーのトリガ判定を返す
          * \return Spaceキーのトリガ判定
          */
         inline bool SpaceClick() const { return _trg & PAD_INPUT_10; }

      private:
         int _key = 0;   //!< 押下判定
         int _trg = 0;   //!< トリガ判定
      };
   }
}