#pragma once
/*****************************************************************//**
 * \file   SpriteServer.h
 * \brief  スプライトを一括管理するクラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBaseRoot.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief スプライト関係
    */
   namespace Sprite {
      /**
       * \class スプライトを一括管理するクラス
       * \brief スプライトを登録し、一括管理する
       */
      class SpriteServer {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief スプライトの登録
          * \param sprite 登録するスプライト
          */
         void Add(std::unique_ptr<SpriteBaseRoot> sprite);
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Render();
         /**
          * \brief スプライトの解放
          */
         void Clear();
         /**
          * \brief スプライトを登録している動的配列の取得
          */
         std::vector<std::unique_ptr<SpriteBaseRoot>>& runSprites() { return _runSprites; };


      private:
         bool _updating{ false };                                      //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<SpriteBaseRoot>> _runSprites;     //!< スプライトを登録する動的配列
         std::vector<std::unique_ptr<SpriteBaseRoot>> _pendingSprites; //!< スプライトの登録を保留する動的配列
      };
   }
}
