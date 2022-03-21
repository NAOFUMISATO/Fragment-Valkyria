#pragma once
/*****************************************************************//**
 * \file   EffectServer.h
 * \brief  エフェクト一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include <unordered_map>
#include <vector>
#include <memory>
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class エフェクト一括管理クラス
       * \brief EffectBaseRootを継承したEffectクラスを一括管理する
       */
      class EffectServer {
      public:
         /**
          * \brief エフェクトを動的配列に追加する
          * \param efc 追加するエフェクト
          */
         void Add(std::unique_ptr<EffectBase> efc);
         /**
          * \brief 動的配列に追加したエフェクトの入力処理を回す
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief 動的配列に追加したエフェクトの更新処理を回す
          */
         void Update();
         /**
          * \brief 動的配列に追加したエフェクトの描画処理を回す
          */
         void Render();
         /**
         * \brief 全ての動的配列の全要素を削除する
         */
         void Clear();
         /**
          * \brief エフェクトの処理を回す動的配列を取得する
          * return エフェクトの処理を回す動的配列
          */
         std::vector<std::unique_ptr<EffectBase>>& runEffects() { return _runEffects; }

      private:
         bool _updating{ false };                                   //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<EffectBase>> _runEffects;      //!< エフェクトの処理を回す用の動的配列
         std::vector<std::unique_ptr<EffectBase>> _pendingEffects;  //!< 保留中のエフェクトを登録しておく動的配列

      };
   }
}
