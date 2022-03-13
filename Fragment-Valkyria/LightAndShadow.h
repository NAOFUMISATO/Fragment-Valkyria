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
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Render();
         void SetDifColor(float red,float green,float blue);
         void SetAmbColor(float red, float green, float blue);
      private:
         Game::GameMain& _gameMain;       //!< ゲーム本体クラスの参照
         int _lightHandle{ -1 };          //!< 光源ハンドルのペア型
         Vector4 _lightPosition{ 0,0,0 }; //!< 光源ハンドル位置のペア型
         int _shadowHandle{ -1 };         //!< 落ち影ハンドル
      };
   }

}
