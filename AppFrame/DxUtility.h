#pragma once
/*****************************************************************//**
 * \file   DxUtility.h
 * \brief  DxLib�֐��p�Z�p�N���X
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
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
      /**
       * \brief ����Vector4�N���X����DxLib::VECTOR�\���̂ւ̕ϊ�
       * \param vector ����Vector4�N���X
       */
      VECTOR ToDX(Math::Vector4& vector);
      /**
       * \brief DxLib::VECTOR�\���̂��玩��Vector4�N���X�ւ̕ϊ�
       * \param vector DxLib::VECTOR�\����
       */
      Math::Vector4 ToMath(VECTOR& vector);
      /**
       * \brief ����Matrix44�N���X����DxLib::MATRIX�\���̂ւ̕ϊ�
       * \param matrix ����Matrix44�N���X
       */
      MATRIX ToDX(Math::Matrix44& matrix);
      /**
       * \brief DxLib::MATRIX�\���̂��玩��Matirix44�N���X�ւ̕ϊ�
       * \param matrix DxLib::MATRIX�\����
       */
      Math::Matrix44 ToMath(MATRIX& matrix);

   }
}