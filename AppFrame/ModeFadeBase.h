#pragma once
/*****************************************************************//**
 * \file   ModeFadeBase.h
 * \brief  モードのフェード基底クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeBase.h"
namespace {
   constexpr auto BOX_WIDTH = 1920;  //!< DxLib::DrawBox横サイズ
   constexpr auto BOX_HEIGHT = 1080; //!< DxLib::DrawBox縦サイズ
}
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
      class ModeFadeBase :public ModeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションフレーム基底クラスの参照
          */
         ModeFadeBase(Game::GameBase& appBase);
         /**
          * \brief デストラクタ
          */
         virtual ~ModeFadeBase();
         /**
          * \brief 初期化処理
          */
         virtual void Init() override;
         /**
          * \brief 入口処理
          */
         virtual void Enter() override;
         /**
          * \brief 更新処理
          */
         virtual void Update() override;
         /**
          * \brief 描画処理
          */
         virtual void Render() override;

      protected:
         int _width{ BOX_WIDTH };      //!< 画面暗転用DxLib::DrawBox横サイズ
         int _height{ BOX_HEIGHT };    //!< 画面暗転用DxLib::DrawBox縦サイズ
         unsigned int _color{ 0 };     //!< 画面暗転用DxLib::GetColorのRGB
         float _alpha{ 0 };            //!< 画面フェード用透明度
         float _deltaAlpha{ 0 };       //!< 毎フレームの暗転速度
      };
   }
}