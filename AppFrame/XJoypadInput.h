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
         inline bool DUpPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_UP]; }
         /**
          * \brief XInputジョイパッドの十字キー下の押下判定を返す
          * \return 十字キー下の押下判定
          */
         inline bool DDownPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_DOWN]; }
         /**
          * \brief XInputジョイパッドの十字キー左の押下判定を返す
          * \return 十字キー左の押下判定
          */
         inline bool DLeftPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_LEFT]; }
         /**
          * \brief XInputジョイパッドの十字キー右の押下判定を返す
          * \return 十字キー右の押下判定
          */
         inline bool DRightPress() const { return _key.Buttons[XINPUT_BUTTON_DPAD_RIGHT]; }
         /**
          * \brief XInputジョイパッドのSTARTボタンのトリガ判定を返す
          * \return STARTボタンのトリガ判定
          */
         inline bool StartClick() const { return _trg.Buttons[XINPUT_BUTTON_START]; }
         /**
          * \brief XInputジョイパッドのBACKボタンのトリガ判定を返す
          * \return BACKボタンのトリガ判定
          */
         inline bool BackClick() const { return _trg.Buttons[XINPUT_BUTTON_BACK]; }
         /**
          * \brief XInputジョイパッドの左スティック押し込みのトリガ判定を返す
          * \return 左スティック押し込みのトリガ判定
          */
         inline bool LeftStickClick() const { return _trg.Buttons[XINPUT_BUTTON_LEFT_THUMB]; }
         /**
          * \brief XInputジョイパッドの右スティック押し込みのトリガ判定を返す
          * \return 右スティック押し込みのトリガ判定
          */
         inline bool RightStickClick() const { return _trg.Buttons[XINPUT_BUTTON_RIGHT_THUMB]; }
         /**
          * \brief LBボタンのトリガ判定を返す
          * \return LBボタンのトリガ判定
          */
         inline bool LBClick() const { return _trg.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]; }
         /**
          * \brief RBボタンのトリガ判定を返す
          * \return RBボタンのトリガ判定
          */
         inline bool RBClick() const { return _trg.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]; }
         /**
          * \brief Aボタンのトリガ判定を返す
          * \return Aボタンのトリガ判定
          */
         inline bool AClick() const { return _trg.Buttons[XINPUT_BUTTON_A]; }
         /**
          * \brief Bボタンのトリガ判定を返す
          * \return Bボタンのトリガ判定
          */
         inline bool BClick() const { return _trg.Buttons[XINPUT_BUTTON_B]; }
         /**
          * \brief Xボタンのトリガ判定を返す
          * \return Xボタンのトリガ判定
          */
         inline bool XClick() const { return _trg.Buttons[XINPUT_BUTTON_X]; }
         /**
          * \brief Yボタンのトリガ判定を返す
          * \return Yボタンのトリガ判定
          */
         inline bool YClick() const { return _trg.Buttons[XINPUT_BUTTON_Y]; }
         /**
          * \brief 左スティックのX軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのX軸入力量
          */
         inline short LeftStickX() const { return _key.ThumbLX; }
         /**
          * \brief 左スティックのY軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのY軸入力量
          */
         inline short LeftStickY() const { return _key.ThumbLY; }
         /**
          * \brief 右スティックのX軸入力量を返す(-32768 ～ 32767)
          * \return 左スティックのX軸入力量
          */
         inline short RightStickX() const { return _key.ThumbRX; }
         /**
          * \brief 右スティックのY軸入力量を返す(-32768 ～ 32767)
          * \return 右スティックのY軸入力量
          */
         inline short RightStickY() const { return _key.ThumbRY; }
         /**
          * \brief 左トリガーの入力量を返す(0 ～ 255)
          * \return 左トリガーの入力量
          */
         inline unsigned char LeftTrigger() const { return _key.LeftTrigger; }
         /**
          * \brief 右トリガーの入力量を返す(0 ～ 255)
          * \return 右トリガーの入力量
          */
         inline unsigned char RightTrigger() const { return _key.RightTrigger; }

      private:
         XINPUT_STATE _key{ 0 };   //!< XInputジョイパッドの押下判定
         XINPUT_STATE _trg{ 0 };   //!< XInputジョイパッドのトリガ判定
      };
   }
}
