#pragma once
/*****************************************************************//**
 * \file   Reticle.h
 * \brief  レティクル描画クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief プレイヤー関係
    */
   namespace Player {
      /**
       * \class レティクル描画クラス
       * \brief プレイヤーの状態に合わせ、レティクルの描画を行う
       */
      class Reticle :public Sprite::SpriteBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         Reticle(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:Sprite
          */
         virtual SpriteType GetSprType()const { return SpriteType::Sprite; }

      private:
         bool _isAim{ false };   //!< プレイヤーがエイムしているかのフラグ
      };
   }
}
