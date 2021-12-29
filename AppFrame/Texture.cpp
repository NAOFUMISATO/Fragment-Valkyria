
/*****************************************************************//**
 * \file   Texture.cpp
 * \brief  �摜�f�[�^
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "Texture.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      Texture::Texture(std::string_view textureName, int allNum, int xNum, int yNum, int xSize, int ySize) :_textureName(textureName),
         _allNum(allNum), _xNum(xNum), _yNum(yNum), _xSize(xSize), _ySize(ySize) {
      }
      Texture::Texture() {
         _textureName = "";
         _allNum = 0;
         _xNum = 0;
         _yNum = 0;
         _xSize = 0;
         _ySize = 0;
      }
   }
}
