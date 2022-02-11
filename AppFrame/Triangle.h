#pragma once
/*****************************************************************//**
 * \file   Triangle.h
 * \brief  三角形クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief データ関係
    */
   namespace Data {
      /**
       * \class 三角形クラス
       * \brief 三角形描画用のデータ
       */
      class Triangle {
         using Apex = std::pair<double, double>;
      public:
         /**
          * \brief コンストラクタ
          * \param firstPos 三角形の頂点1
          * \param secondPos 三角形の頂点2
          * \param thirdPos 三角形の頂点3
          */
         Triangle(Apex firstPos, Apex secondPos, Apex thirdPos) :
            _firstPos{ firstPos }, _secondPos{ secondPos }, _thirdPos{ thirdPos }{
         };
         /**
          * \brief 空のコンストラクタ
          */
         Triangle() {};
         /**
          * \brief 三角形のデータの設定
          * \param firstPos
          * \param secondPos
          * \param thirdPos
          */
         void SetTriangleParams(Apex firstPos, Apex secondPos, Apex thirdPos) {
            _firstPos = firstPos, _secondPos = secondPos, _thirdPos = thirdPos;
         }
         /**
          * \brief 三角形のデータの取得
          * \return 三角形のデータ
          */
         std::tuple<Apex, Apex, Apex> GetTriangleParams() { return { _firstPos ,_secondPos,_thirdPos }; }

      private:
         Apex _firstPos{ 0.0,0.0 };   //!< 三角形の頂点データ1
         Apex _secondPos{ 0.0,0.0 };  //!< 三角形の頂点データ2
         Apex _thirdPos{ 0.0,0.0 };   //!< 三角形の頂点データ3
      };

   }
}
