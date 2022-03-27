
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
         std::array<unsigned char, 3> color = { _red,_green,_blue };
         std::string redCode;
         std::string greenCode;
         std::string blueCode;
         for (int i = 0; i < color.size(); i++) {
            std::string front;
            auto div = color[i] / 16;
            switch (div) {
            default:
               front = std::to_string(div);
               break;
            case 10:
               front = "A";
               break;
            case 11:
               front = "B";
               break;
            case 12:
               front = "C";
               break;
            case 13:
               front = "D";
               break;
            case 14:
               front = "E";
               break;
            case 15:
               front = "F";
               break;
            }
            std::string back;
            auto rem = color[i] % 16;

            switch (rem) {
            default:
               back = std::to_string(rem);
               break;
            case 10:
               back = "A";
               break;
            case 11:
               back = "B";
               break;
            case 12:
               back = "C";
               break;
            case 13:
               back = "D";
               break;
            case 14:
               back = "E";
               break;
            case 15:
               back = "F";
               break;
            }

            if (i == 0) {
               redCode = front + back;
            }
            if (i == 1) {
               greenCode = front + back;
            }
            if (i == 2) {
               blueCode = front + back;
            }
         }
         auto strCode = "0x" + redCode + greenCode + blueCode;
         unsigned int colorCode = std::stoi(strCode, nullptr, 16);
         return colorCode;
      }
   }
}
