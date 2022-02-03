#pragma once
/*****************************************************************//**
 * \file   ModeFadeBase.h
 * \brief  モードのフェード基底クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeBaseRoot.h"
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードのフェード基底クラス
       * \brief フェード用モードの基底クラス
       */
      class ModeFadeBase :public ModeBaseRoot {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションフレーム基底クラスの参照
          */
         ModeFadeBase(Game::GameBase& gameBase);
         /**
          * \brief 初期化処理
          */
         void Init()override {};
         /**
          * \brief 描画処理
          */
         void Render() override;

      protected:
         float _alpha{ 0 };            //!< 画面フェード用透明度
         float _deltaAlpha{ 0 };       //!< 毎フレームの暗転速度
      };
   }
}