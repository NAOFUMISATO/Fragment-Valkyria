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
          */
         ModeFadeIn();
         /**
          * \brief 初期化処理
          */
         void Init()override {};
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
