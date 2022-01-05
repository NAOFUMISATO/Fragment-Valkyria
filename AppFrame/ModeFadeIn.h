#pragma once
/*****************************************************************//**
 * \file   ModeFadeIn.h
 * \brief  モードのフェードインクラス
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
       * \class モードのフェードインクラス
       * \brief モード遷移時に呼び出す
       */
      class ModeFadeIn : public ModeFadeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションフレーム基底クラスの参照
          */
         ModeFadeIn(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         ~ModeFadeIn() = default;
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
