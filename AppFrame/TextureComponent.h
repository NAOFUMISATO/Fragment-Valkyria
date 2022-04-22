#pragma once
/*****************************************************************//**
 * \file   TextureComponent.h
 * \brief  画像及びビルボード簡易描画関数クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <vector>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   // 二重インクルード防止
   namespace Math {
      class Vector4;
   }
   /**
    * \brief 画像描画関係
    */
   namespace Texture {
      /**
       * \class 画像及びビルボード簡易描画関数クラス
       * \brief 画像の描画及びアニメーションを簡易的に行う
       */
      class TextureComponent {
      public:
         /**
          * \brief コンストラクタ
          */
         TextureComponent();
         /**
          * \brief 画像の描画を簡易的に行う
          * \param x 画像左上X座標
          * \param y 画像左上Y座標
          * \param scale 拡大率
          * \param angle 角度(弧度法)
          * \param handle 画像ハンドル
          */
         void DrawTexture(int x, int y, double scale, double angle, int handle);
         /**
          * \brief 画像のアニメーション描画を簡易的に行う
          * \param x 画像左上X座標
          * \param y 画像左上Y座標
          * \param scale 拡大率
          * \param angle 角度(弧度法)
          * \param handles 画像ハンドルの動的配列
          * \param animeSpeed アニメーションスピード
          */
         void DrawTexture(int x, int y, double scale, double angle, std::vector<int> handles, int animeSpeed);
         /**
          * \brief 画像の描画を詳細設定を行い、描画する
          * \param x 画像左上X座標
          * \param y 画像左上Y座標
          * \param cx 画像の回転中心位置X座標(画像左上が0)
          * \param cy 画像の回転中心位置Y座標(画像左上が0)
          * \param scale 拡大率
          * \param angle 角度(弧度法)
          * \param handle 画像ハンドル
          * \param transFlag 背景透過フラグ
          * \param turnFrag 反転判定
          */
         void TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, int handle, bool transFlag, bool turnFrag);
         /**
          * \brief 画像のアニメーション描画を詳細設定を行い、描画する
          * \param x 画像左上X座標
          * \param y 画像左上Y座標
          * \param cx 画像の回転中心位置X座標(画像左上が0)
          * \param cy 画像の回転中心位置Y座標(画像左上が0)
          * \param scale 拡大率
          * \param angle 角度(弧度法)
          * \param handles 画像ハンドルの動的配列
          * \param animeSpeed アニメーションスピード
          * \param transFlag 背景透過フラグ
          * \param turnFrag 反転判定
          */
         void TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, std::vector<int> handles, int animeSpeed, bool transFlag,bool turnFrag);
         /**
          * \brief 画像のビルボード描画を簡易的に行う
          * \param pos 描画座標(足下基準)
          * \param scale 拡大率(0.0～1.0)
          * \param angle 角度(弧度法)
          * \param handle 画像ハンドル
          */
         void DrawBillBoard(Math::Vector4 pos, double scale, double angle, int handle);
         /**
          * \brief 画像のビルボードアニメーション描画を簡易的に行う
          * \param pos 描画座標(足下基準)
          * \param scale 拡大率(0.0～1.0)
          * \param angle 角度(弧度法)
          * \param handles 画像ハンドルの動的配列
          * \param animeSpeed アニメーションスピード
          */
         void DrawBillBoard(Math::Vector4 pos,double scale, double angle, std::vector<int> handles, int animeSpeed);
         /**
          * \brief 画像のビルボード描画を詳細設定を行い、描画する
          * \param pos 描画座標(足下基準)
          * \param cx 基準位置X座標
          * \param cy 基準位置Y座標
          * \param scale 拡大率(0.0～1.0)
          * \param angle 角度(弧度法)
          * \param handle 画像ハンドル
          */
         void TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double scale, double angle, int handle);
         /**
          * \brief 画像のビルボードアニメーションを詳細設定を行い、描画する
          * \param pos 描画座標(足下基準)
          * \param cx 基準位置X座標
          * \param cy 基準位置Y座標
          * \param scale 拡大率(0.0～1.0)
          * \param angle 角度(弧度法)
          * \param handles 画像ハンドルの動的配列
          * \param animeSpeed アニメーションスピード
          */
         void TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double scale, double angle, std::vector<int> handles, int animeSpeed);
      };
   }
}
