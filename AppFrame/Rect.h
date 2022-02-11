#pragma once
/*****************************************************************//**
 * \file   Rect.h
 * \brief  矩形クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame{
   /**
    * \brief データ関係
    */
   namespace Data {
      /**
       * \class 矩形クラス
       * \brief 矩形描画用のデータ
       */
      class Rect {
      public:
         /**
          * \brief コンストラクタ
          * \param left 矩形左上X座標
          * \param top 矩形左上Y座標
          * \param right 矩形右下X座標
          * \param bottom 矩形右下Y座標
          */
         Rect(double left, double top,double right, double bottom) :
            _left(left),_top(top),_right(right),_bottom(bottom){
         };
         /**
          * \brief 空のコンストラクタ
          */
         Rect() {};
         /**
          * \brief 矩形のデータの設定
          * \param left 矩形左上X座標
          * \param top 矩形左上Y座標
          * \param right 矩形右下X座標
          * \param bottom 矩形右下Y座標
          */
         void SetRectParams(double left, double top, double right, double bottom){
            _left = left, _top = top, _right = right, _bottom = bottom;
         }
         /**
          * \brief 矩形のデータの取得
          * \return 矩形のデータ
          */
         std::tuple<double, double, double, double> GetRectParams() { return {_left,_top,_right,_bottom}; }
         /**
          * \brief 矩形の右下X座標の設定
          * \param right 矩形の右下X座標
          */
         void right(double right) { _right = right; }

      private:
         double _left{ 0 };   //!< 矩形左上X座標
         double _top{ 0 };    //!< 矩形左上Y座標
         double _right{ 0 };  //!< 矩形右下X座標
         double _bottom{ 0 }; //!< 矩形右下Y座標
      };

}
}
