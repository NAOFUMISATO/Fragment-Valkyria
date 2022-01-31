#pragma once
/*****************************************************************//**
 * \file   LightAndShadow.h
 * \brief  ライティング及び落ち影の処理を行うクラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief ライティング関係
    */
   namespace Lighting {
      /**
       * \class ライティング及び落ち影の処理を行うクラス
       * \brief 光源処理及びシャドウマップの描画を管理する
       */
      class LightAndShadow {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         LightAndShadow(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init();
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Render();

      private:
         Game::GameMain& _gameMain;                                      //!< ゲーム本体クラスの参照
         std::pair<int, int> _lightHandles{ -1,-1 };                     //!< 光源ハンドルのペア型
         std::pair<Vector4, Vector4> _lightPositions{ {0,0,0},{0,0,0} }; //!< 光源ハンドル位置のペア型
         int _shadowHandle{ -1 };                                        //!< 落ち影ハンドル
      };
   }

}
