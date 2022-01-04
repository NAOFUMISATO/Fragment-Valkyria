#pragma once
/*****************************************************************//**
 * \file   XJoypadInput.h
 * \brief  XInputジョイパッドの入力管理クラス
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
    * \brief 入力管理名前空間
    */
   namespace Input {
      /**
       * \class XInputジョイパッドの入力管理クラス
       * \brief DxLib::GetJoypadXInputStateで入力を受け取り,管理する
       */
      class XJoypadInput {
      public:
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief XInputジョイパッドの十字キー上の押下判定を返す
          * \return 十字キー上の押下判定
          */
         inline bool DUpPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_UP]; }
         /**
          * \brief XInputジョイパッドの十字キー下の押下判定を返す
          * \return 十字キー下の押下判定
          */
         inline bool DDownPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_DOWN]; }
         /**
          * \brief XInputジョイパッドの十字キー左の押下判定を返す
          * \return 十字キー左の押下判定
          */
         inline bool DLeftPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_LEFT]; }
         /**
          * \brief XInputジョイパッドの十字キー右の押下判定を返す
          * \return 十字キー右の押下判定
          */
         inline bool DRightPress() { return _key.Buttons[XINPUT_BUTTON_DPAD_RIGHT]; }
         /**
          * \brief XInputジョイパッドのSTARTボタンのトリガ判定を返す
          * \return STARTボタンのトリガ判定
          */
         inline bool StartClick() { return _trg.Buttons[XINPUT_BUTTON_START]; }
         /**
          * \brief XInputジョイパッドのBACKボタンのトリガ判定を返す
          * \return BACKボタンのトリガ判定
          */
         inline bool BackClick() { return _trg.Buttons[XINPUT_BUTTON_BACK]; }
         /**
          * \brief XInputジョイパッドの左スティック押し込みのトリガ判定を返す
          * \return 左スティック押し込みのトリガ判定
          */
         inline bool LeftStickClick() { return _trg.Buttons[XINPUT_BUTTON_LEFT_THUMB]; }
         /**
          * \brief XInputジョイパッドの右スティック押し込みのトリガ判定を返す
          * \return 右スティック押し込みのトリガ判定
          */
         inline bool RightStickClick() { return _trg.Buttons[XINPUT_BUTTON_RIGHT_THUMB]; }
         /**
          * \brief LBボタンのトリガ判定を返す
          * \return LBボタンのトリガ判定
          */
         inline bool LBClick() { return _trg.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]; }
         /**
          * \brief RBボタンのトリガ判定を返す
          * \return RBボタンのトリガ判定
          */
         inline bool RBClick() { return _trg.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]; }
         /**
          * \brief Aボタンのトリガ判定を返す
          * \return Aボタンのトリガ判定
          */
         inline bool AClick() { return _trg.Buttons[XINPUT_BUTTON_A]; }
         /**
          * \brief Bボタンのトリガ判定を返す
          * \return Bボタンのトリガ判定
          */
         inline bool BClick() { return _trg.Buttons[XINPUT_BUTTON_B]; }
         /**
          * \brief Xボタンのトリガ判定を返す
          * \return Xボタンのトリガ判定
          */
         inline bool XClick() { return _trg.Buttons[XINPUT_BUTTON_X]; }
         /**
          * \brief Yボタンのトリガ判定を返す
          * \return Yボタンのトリガ判定
          */
         inline bool YClick() { return _trg.Buttons[XINPUT_BUTTON_Y]; }
         /**
          * \brief 左スティックのX軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのX軸入力量
          */
         inline bool LeftStickX() { return _key.ThumbLX; }
         /**
          * \brief 左スティックのY軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのY軸入力量
          */
         inline bool LeftStickY() { return _key.ThumbLY; }
         /**
          * \brief 右スティックのX軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのX軸入力量
          */
         inline bool RightStickX() { return _key.ThumbRX; }
         /**
          * \brief 右スティックのY軸入力量を返す(-32768 ～ 32767)
          * \return 右スティックのY軸入力量
          */
         inline bool RightStickY() { return _key.ThumbRY; }
         /**
          * \brief 左トリガーの入力量を返す(0 ～ 255)
          * \return 左トリガーの入力量
          */
         inline bool LeftTrigger() { return _key.LeftTrigger; }
         /**
          * \brief 右トリガーの入力量を返す(0 ～ 255)
          * \return 右トリガーの入力量
          */
         inline bool RightTrigger() { return _key.RightTrigger; }

      private:
         XINPUT_STATE _key{ 0 };   //!< XInputジョイパッドの押下判定
         XINPUT_STATE _trg{ 0 };   //!< XInputジョイパッドのトリガ判定
      };
   }
}
