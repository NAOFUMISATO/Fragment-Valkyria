#pragma once
/*****************************************************************//**
 * \file   LightAndShadow.h
 * \brief  ライティング及び落ち影の処理を行うクラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
#include "ParamLightShadow.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
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
          */
         LightAndShadow();
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Render();
         /**
          * \brief ライティングの拡散光色の設定
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetDifColor(float red,float green,float blue);
         /**
          * \brief ライティングの環境光色の設定
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetAmbColor(float red, float green, float blue);

      private:
         std::unique_ptr<Param::ParamLightShadow> _param;//!< ライティング処理を行うクラスの値管理クラス
         int _lightHandle{ -1 };                         //!< 光源ハンドルのペア型
         int _shadowHandle{ -1 };                        //!< 落ち影ハンドル
         Vector4 _lightPosition{ 0,0,0 };                //!< 光源ハンドル位置のペア型
      };
   }

}
