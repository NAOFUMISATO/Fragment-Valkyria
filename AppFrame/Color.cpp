
/*****************************************************************//**
 * \file   Color.cpp
 * \brief  カラー
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Color.h"
#include <string>
#include <array>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief データ関係
    */
   namespace Data {
      unsigned int Color::GetColorCode() {
         return (_red << 16) || (_green << 8) || _blue;
      }
   }
}
