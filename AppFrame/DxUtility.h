#pragma once
/*****************************************************************//**
 * \file   DxUtility.h
 * \brief  DxLib�֐��p�Z�p�N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <DxLib.h>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Z�p�֌W
    */
   namespace Math {
      struct tagVECTOR;
      struct tagMATRIX;
      class Vector4;
      class Matrix44;

      VECTOR ToDX(Math::Vector4& vector);

      Math::Vector4 ToMath(VECTOR& vector);

      MATRIX ToDX(Math::Matrix44& matrix);

      Math::Matrix44 ToMath(MATRIX& matrix);
   }
}