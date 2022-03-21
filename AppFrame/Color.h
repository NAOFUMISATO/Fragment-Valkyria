#pragma once
/*****************************************************************//**
 * \file   Color.h
 * \brief  カラークラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief 各データ関係
    */
   namespace Data {
      /**
       * \class カラークラス
       * \brief カラー値の管理、カラーコードの取得を行う
       */
      class Color {
      public:
         /**
          * \brief コンストラクタ(透明値含む)
          * \param r 赤値
          * \param g 緑値
          * \param b 青値
          * \param a 透明値
          */
         Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
            _red(r), _green(g), _blue(b), _alpha(a) {
         };
         /**
          * \brief コンストラクタ(透明値除く)
          * \param r 赤値
          * \param g 緑値
          * \param b 青値
          */
         Color(unsigned char r, unsigned char g, unsigned char b) :
            _red(r), _green(g), _blue(b) {
         };
         /**
          * \brief 空のコンストラクタ
          */
         Color() {};
         /**
          * \brief デフォルトデストラクタ
          */
         ~Color() = default;

         /**
          * \brief カラーコードの取得
          * \return カラーコード
          */
         unsigned int GetColorCode();
         /**
          * \brief カラー値の取得(透明度含む)
          * \return 透明度含むカラー値
          */
         std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> GetColorA() const { 
            return std::make_tuple(_red, _green, _blue, _alpha); 
         }
         /**
          * \brief カラー値の取得(透明度除く)
          * \return 透明度除くカラー値
          */
         std::tuple<unsigned char, unsigned char, unsigned char> GetColor() const {
            return std::make_tuple(_red, _green, _blue);
         }
         /**
          * \brief カラー値の設定(透明度含む)
          * \param r 赤値
          * \param g 緑値
          * \param b 青値
          * \param a 透明値
          */
         void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
            _red = r;_green = g;_blue = b;_alpha =a;
         }
         /**
          * \brief カラー値の設定(透明度除く)
          * \param r 赤値
          * \param g 緑値
          * \param b 青値
          */
         void SetColor(unsigned char r, unsigned char g, unsigned char b) {
            _red = r; _green = g; _blue = b;
         }
         /**
          * \brief 赤値の取得
          * \return 赤値
          */
         unsigned char red() const { return _red; }
         /**
          * \brief 赤値の設定
          * \param r 設定する赤値
          */
         void red(unsigned char r) { _red = r; }
         /**
          * \brief 緑値の取得
          * \return 緑値
          */
         unsigned char green() const { return _green; }
         /**
          * \brief 緑値の設定
          * \param g 設定する緑値
          */
         void green(unsigned char g) { _green = g; }
         /**
          * \brief 青値の取得
          * \return 青値
          */
         unsigned char blue() const { return _blue; }
         /**
          * \brief 青値の設定
          * \param b 設定する青値
          */
         void blue(unsigned char b) { _blue = b; }
         /**
          * \brief 透明度の取得
          * \return 透明度
          */
         unsigned char alpha() const { return _alpha; }
         /**
          * \brief 透明度の設定
          * \param a 設定する透明度
          */
         void alpha(unsigned char a) { _alpha = a; }

      private:
         unsigned char _red{ 255 };    //!< 赤値
         unsigned char _green{ 255 };  //!< 緑値
         unsigned char _blue{ 255 };   //!< 青値
         unsigned char _alpha{ 255 };  //!< 透明度

      };
   }
}
