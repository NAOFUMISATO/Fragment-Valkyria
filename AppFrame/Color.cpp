
/*****************************************************************//**
 * \file   Color.cpp
 * \brief  �J���[
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Color.h"
#include <string>
#include <array>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �f�[�^�֌W
    */
   namespace Data {
      unsigned int Color::GetColorCode() {
         return (_red << 16) || (_green << 8) || _blue;
      }
   }
}
