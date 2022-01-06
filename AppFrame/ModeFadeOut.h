#pragma once
/*****************************************************************//**
 * \file   ModeFadeOut.h
 * \brief  モードのフェードアウトクラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeFadeBase.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードのフェードアウトクラス
       * \brief モードの遷移時に呼び出し、真下のシーンを除外する
       */
      class ModeFadeOut : public ModeFadeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションフレームの基底クラスの参照
          */
         ModeFadeOut(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         ~ModeFadeOut() = default;
         /**
          * \brief 入口処理
          */
         void Enter() override;
         /**
          * \brief 更新処理
          */
         void Update() override;
      };
   }
}