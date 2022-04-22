#pragma once
/*****************************************************************//**
 * \file   ClearTime.h
 * \brief  クリア時間クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria{
   /**
    * \brief クリア関係
    */
   namespace Clear {
      class ClearTime :public Sprite::SpriteBase{
         using Uint = unsigned int;
      public:
         /**
          * \brief コンストラクタ
          */
         ClearTime();
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:クリア時間
          */
         virtual SpriteType GetSprType() const { return SpriteType::ClearTime; }

      private:
         std::tuple<Uint, Uint, Uint, Uint, Uint> _xPositions{ 0,0,0,0,0 }; //!< クリア時間1桁ごとのX座標
         std::tuple<Uint, Uint, Uint, Uint, Uint> _animeNos{ 0,0,0,0,0 };   //!< クリア時間1桁ごとのアニメ番号
      };
   }
}
