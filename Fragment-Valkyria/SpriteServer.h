#pragma once
/*****************************************************************//**
 * \file   SpriteServer.h
 * \brief  スプライトを一括管理するクラス
 *
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief スプライト関係
    */
   namespace Sprite {
      /**
       * \class スプライトを一括管理するクラス
       * \brief スプライトを登録し、一括管理する
       */
      class SpriteServer {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief スプライトの登録
          * \param sprite 登録するスプライト
          */
         void Add(std::unique_ptr<SpriteBase> sprite);
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
         std::vector<std::unique_ptr<SpriteBase>>& runSprites() { return _runSprites; };

      private:
         bool _updating{ false };                                      //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<SpriteBase>> _runSprites;     //!< スプライトを登録する動的配列
         std::vector<std::unique_ptr<SpriteBase>> _pendingSprites; //!< スプライトの登録を保留する動的配列
      };
   }
}
