#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  モード基底クラス
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include "ParamBase.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   // 二重インクルード防止
   namespace Resource {
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Sound {
      class SoundComponent;
   }
   namespace Texture {
      class TextureComponent;
   }
   namespace Sprite {
      class SpriteServer;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      //二重インクルード防止
      class ModeServer;
      /**
       * \class 各モードの基底クラス
       * \brief ModeServerに登録するモードの基底クラス
       *        このクラスを継承してモードを定義する
       */
      class ModeBase {
      public:
         /**
          * \brief コンストラクタ
          */
         ModeBase() {};
         /**
          * \brief 初期化処理
          */
         virtual void Init() = 0;
         /**
          * \brief 入口処理
          */
         virtual void Enter() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 更新処理
          */
         virtual void Render() {};
         /**
          * \brief 出口処理
          */
         virtual void Exit() {};

         /**
          * \brief フェード時間の設定
          * \param fadeType フェード時間を指定する文字
          */
         inline void fadeType(char fadeType) { _fadeType = fadeType; }

      protected:
         std::unique_ptr<Param::ParamBase> _param; //!< 値管理用クラスのポインタ
         char _fadeType{ 'M' };                    //!< フェード時間を指定する文字
      };
   }
}