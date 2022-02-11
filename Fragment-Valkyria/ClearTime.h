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
          * \param gameMain ゲーム本体クラスの参照
          */
         ClearTime(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラス
          */
         void Input(AppFrame::Input::InputManager& input)override;
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
          * \return 種別Sprite
          */
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

      private:
         std::tuple<Uint, Uint, Uint, Uint, Uint> _xPositions{ 0,0,0,0,0 }; //!< クリア時間1桁ごとのX座標
         std::tuple<Uint, Uint, Uint, Uint, Uint> _animeNos{ 0,0,0,0,0 };   //!< クリア時間1桁ごとのアニメ番号
      };
   }
}
