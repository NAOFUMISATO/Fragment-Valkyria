#pragma once
/*****************************************************************//**
 * \file   Texture.h
 * \brief  画像データクラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <string_view>
#include <vector>
#include <tuple>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {
      /**
       * \class 画像データクラス
       * \brief DxLib::LoadDivGraph関数に必要な画像情報をコピーする
       */
      class Texture {
      public:
         /**
          * \brief コンストラクタ
          * \param fileName ファイル名
          * \param allNum 全ての画像枚数
          * \param xNum 画像の横分割数
          * \param yNum 画像の横分割数
          * \param xSize 分割された画像一つの横サイズ
          * \param ySize 分割された画像一つの縦サイズ
          */
         Texture(std::string_view fileName, int allNum, int xNum, int yNum, int xSize, int ySize) :
            _fileName(fileName),_allNum(allNum), _xNum(xNum), _yNum(yNum), _xSize(xSize), _ySize(ySize) {
         };
         /**
          * \brief 空のコンストラクタ
          */
         Texture() {};
         /**
          * \brief  ファイル名の取得
          * \return 画像ファイル名
          */
         inline std::string_view fileName() const { return _fileName; }
         /**
          * \brief  DxLib::LoadDivGraphに必要な情報を返す
          * \return 画像の総分割数,画像の横分割数,画像の縦分割数,分割された画像一つの横サイズ,分割された画像一つの縦サイズ
          */
         inline std::tuple<int, int, int, int, int> GetDivParams() const { return { _allNum, _xNum,_yNum, _xSize, _ySize }; }

      private:
         std::string _fileName{ "" };   //!< ファイル名
         int _allNum{ 0 };              //!< 画像の総分割数
         int _xNum{ 0 };                //!< 画像の横分割数
         int _yNum{ 0 };                //!< 画像の縦分割数
         int _xSize{ 0 };               //!< 分割された画像一つの横サイズ
         int _ySize{ 0 };               //!< 分割された画像一つの縦サイズ
      };
   }
}
