#pragma once
/*****************************************************************//**
 * \file   Matrix44.h
 * \brief  4�����s��N���X
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
 * \date   December 2021
 *********************************************************************/
#include <cstdint>
#include <array>
#include <tuple>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Z�p�֌W
    */
   namespace Math {
      // ��d�C���N���[�h�h�~
      class Vector4;

      constexpr uint32_t _rowMax = 4;     //!< �s��
      constexpr uint32_t _columnMax = 4;  //!< ��
      using MatrixArray = std::array<std::array<double, _columnMax>, _rowMax>;   //!< �s��\���̓񎟔z��
      using Quaternion = std::tuple<double, double, double, double>;                     //!< �N�H�[�^�j�I����\��tuple�^
      /**
       * \class 4�����s��N���X
       * \brief �s��v�Z���s������N���X
       */
      class Matrix44 {
      public:
         /**
          * \brief ��̃R���X�g���N�^
          */
         Matrix44();
         /**
          * \brief �R���X�g���N�^
          * \param values �s��\���̓񎟔z��
          */
         Matrix44(const MatrixArray values);
         /**
          * \brief �R�s�[�R���X�g���N�^
          */
         Matrix44(const Matrix44&) = default;
         /**
          * \brief ���[�u�R���X�g���N�^
          */
         Matrix44(Matrix44&&) = default;
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~Matrix44() = default;
         /**
          * \brief �R�s�[
          */
         Matrix44& operator =(const Matrix44&) = default;
         /**
          * \brief ���[�u
          */
         Matrix44& operator =(Matrix44&&) = default;
         /**
          * \brief Matrix44��Matrix44�v�Z�̉��Z�q�̃I�[�o�[���[�h(*)
          * \param rhs Matrix44�N���X
          * \return �v�Z����
          */
         const Matrix44 operator *(const Matrix44 rhs) const;
         /**
          * \brief Matrix44��Vector4�v�Z�̉��Z�q�̃I�[�o�[���[�h(+)
          * \param rhs Vector4�N���X
          * \return �v�Z����
          */
         const Matrix44 operator +(const Vector4 rhs) const;
         /**
          * \brief �s��̒l�̎擾
          * \param row �s
          * \param column ��
          * \return �s��̒l
          */
         const double GetValue(const uint32_t row, const uint32_t column) const { return _rowColumn[row][column]; }
         /**
          * \brief �s��̒l�̐ݒ�
          * \param row �s
          * \param column ��
          * \param value �ݒ肷��l
          * \return �s��̒l
          */
         const void SetValue(const uint32_t row, const uint32_t column, const double value) { _rowColumn[row][column] = value; }
         /**
          * \brief �P�ʍs�����Z
          * \param target �P�ʍs����|����s��
          */
         void Zero(MatrixArray& target) const;
         /**
          * \brief �P�ʍs��̎擾
          */
         void Unit();
         /**
          * \brief �J�����̃r���[�s��̐ݒ�
          * \param position �J�����ʒu�x�N�g��
          * \param target �����_�̃x�N�g��
          * \param up �J�����̏�����x�N�g��
          */
         void LookAt(const Vector4& position, const Vector4& target, const Vector4& up);
         /**
          * \brief �J�����̓����ϊ��̐ݒ�
          * \param fov ����p
          * \param aspect �A�X�y�N�g��
          * \param nearZ �J�����ɍł��߂��\�����E
          * \param farZ �J�����ɍł������\�����E
          */
         void Perspective(const double fov, const double aspect, const double nearZ, const double farZ);
         /**
          * \brief �\���̈�̐ݒ�
          * \param width�@���̈�T�C�Y
          * \param height�@�c�̈�T�C�Y
          */
         void Viewport(const double width, const double height);
         /**
          * \brief ���s�s�����Z
          * \param x x����
          * \param y y����
          * \param z z����
          * \param make �P�ʉ����邩
          */
         void Transfer(const double x, const double y, const double z, bool make);
         /**
          * \brief �g��s�����Z
          * \param x x����
          * \param y y����
          * \param z z����
          * \param make �P�ʉ����邩
          */
         void Scale(const double x, const double y, const double z, bool make);
         /**
          * \brief X����]�s�����Z
          * \param degree x����
          * \param make �P�ʉ����邩
          */
         void RotateX(const double degree, bool make);
         /**
          * \brief Y����]�s�����Z
          * \param degree y����
          * \param make �P�ʉ����邩
          */
         void RotateY(const double degree, bool make);
         /**
          * \brief Z����]�s�����Z
          * \param degree z����
          * \param make �P�ʉ����邩
          */
         void RotateZ(const double degree, bool make);
         /**
          * \brief �x�N�g���̔C�ӎ��ł̉�]
          * \param vec ��]�����擾����ʒu
          * \param degree ��]�p
          * \param make �P�ʉ����邩
          */
         void RotateAnyVec(const Vector4 vec, const double degree, bool make);
         /**
          * \brief 
          * \param vec
          * \param degree
          * \param make
          */
         void RotateAnyVecQuaternion(const Vector4 vec, const double degree, bool make);
         /**
          * \brief ��]�s�����Z
          * \return ��Z�����s��
          */
         const Matrix44 GetRotate() const;

      private:
         /**
          * \brief �x���@�֕ϊ�����sin,cos�̎擾
          * \param degree �x���@�̉�]�p
          * \return �x���@�֕ϊ�����sin,cos
          */
         std::tuple<double, double> GetSinCos(const double degree) const;
         MatrixArray _rowColumn;   //!< �s��\���̓񎟔z��
      };
   }
}
