#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  エフェクトの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   //二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class エフェクトの基底クラス
       * \brief 各エフェクトはこのクラスを派生して定義する
       */
      class EffectBase :public AppFrame::Effect::EffectBaseRoot {
      public:
         /**
          * \brief エフェクトの種別
          */
         enum class EffectType {
            Effect = 0    //!< エフェクト
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectBase(Game::GameMain& gameMain,std::string_view key);

         /**
          * \brief エフェクトの種別の取得
          * \return 派生先で定義
          */
         virtual EffectType GetEfcType() const = 0;

      protected:
         Game::GameMain& _gameMain;   //!< ゲーム本体クラスの参照
      };
}
}
