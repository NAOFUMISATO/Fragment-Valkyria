#pragma once
/*****************************************************************//**
 * \file   Vector4.h
 * \brief  4�����x�N�g���N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <tuple>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Z�p�֌W
    */
   namespace Math {
      //�@��d�C���N���[�h�h�~
      class Matrix44;
      /**
       * \class 4�����x�N�g���N���X
       * \brief �x�N�g���v�Z���s������N���X
       */
      class Vector4 {
      public:
         /**
          * \brief ��̃R���X�g���N�^
          */
         Vector4();
         /**
          * \brief �R���X�g���N�^
          * \param x x����
          * \param y y����
          * \param z z����
          */
         Vector4(const double x, const double y, const double z) : _x(x), _y(y), _z(z), _w(1.0) {}
         /**
          * \brief �R���X�g���N�^
          * \param x x����
          * \param y y����
          * \param z z����
          * \param w w����
          */
         Vector4(const double x, const double y, const double z, double w) : _x(x), _y(y), _z(z), _w(w) {}
         /**
          * \brief �R�s�[�R���X�g���N�^
          */
         Vector4(const Vector4&) = default;
         /**
          * \brief ���[�u�R���X�g���N�^
          */
         Vector4(Vector4&&) = default;
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~Vector4() = default;
         /**
          * \brief �R�s�[
          */
         Vector4& operator =(const Vector4&) = default;
         /**
          * \brief ���[�u
          */
         Vector4& operator =(Vector4&&) = default;
         /**
          * \brief ��r���Z�q�̃I�[�o�[���[�h
          * \param rhs ��r����Vector4�N���X
          * \return ��r����
          */
         bool operator <(const Vector4 rhs) const;
         /**
          * \brief Vector4��Vector4�̉��Z�q�̃I�[�o�[���[�h(+)
          * \param rhs Vector4�N���X
          * \return �v�Z����
          */
         const Vector4 operator +(const Vector4& rhs) const;
         /**
          * \brief Vector4��Vector4�̉��Z�q�̃I�[�o�[���[�h(-)
          * \param rhs Vector4�N���X
          * \return �v�Z����
          */
         const Vector4 operator -(const Vector4& rhs) const;
         /**
          * \brief Vector4��Vector4�̉��Z�q�̃I�[�o�[���[�h(*)
          * \param rhs Vector4�N���X
          * \return �v�Z����
          */
         const Vector4 operator *(const Vector4& rhs) const;
         /**
          * \brief Vector4��double�^�̉��Z�q�̃I�[�o�[���[�h(*)
          * \param rhs double�̒l
          * \return �v�Z����
          */
         const Vector4 operator *(const double rhs) const;
         /**
          * \brief Vector4��double�^�̉��Z�q�̃I�[�o�[���[�h(/)
          * \param rhs double�̒l
          * \return �v�Z����
          */
         const Vector4 operator /(const double rhs) const;
         /**
          * \brief Vector4��Matrix44�̉��Z�q�̃I�[�o�[���[�h(*)
          * \param rhs Matrix44�N���X
          * \return �v�Z����
          */
         const Vector4 operator *(const Matrix44 rhs) const;
         /**
          * \brief Vector4�N���X�̐ݒ�
          * \param vector �ݒ肷��Vector4�N���X
          */
         void Set(const Vector4& vector);
         /**
          * \brief Vector4�N���X�̐ݒ�
          * \param x �ݒ肷��x����
          * \param y �ݒ肷��y����
          * \param z �ݒ肷��z����
          */
         void Set(const double x, const double y, const double z);
         /**
          * \brief Vector4�N���X�̉��Z
          * \param vector ���Z����Vector4�N���X
          */
         void Add(const Vector4& vector);
         /**
          * \brief Vector4�N���X�̉��Z
          * \param x ���Z����x����
          * \param y ���Z����y����
          * \param z ���Z����z����
          */
         void Add(const double x, const double y, const double z);
         /**
          * \brief Vector4�N���X��3���������̎擾
          * \return xyz����
          */
         inline const std::tuple<double, double, double> GetVec3() const { return { _x, _y, _z }; }
         /**
          * \brief Vector4�N���X��3���������̐ݒ�
          * \param x x����
          * \param y y����
          * \param z z����
          */
         inline void SetVec3(double x, double y, double z) { _x = x, _y = y, _z = z; }
         /**
          * \brief Vector4�N���X��2���������̎擾
          * \return xy����
          */
         inline const std::pair<double, double> GetVec2() const { return { _x,_y }; }
         /**
          * \brief Vector4�N���X��2���������̐ݒ�
          * \param x x����
          * \param y y����
          */
         inline void SetVec2(double x, double y) { _x = x, _y = y; }
         /**
          * \brief y�����̎擾
          * \return y����
          */
         inline const double GetY() const { return _y; }
         /**
         * \brief z�����̎擾
         * \return z����
         */
         inline const double GetZ() const { return _z; }
         /**
          * \brief w�����̎擾
          * \return w����
          */
         inline const double GetW() const { return _w; }
         /**
          * \brief y�����̎擾
          * \return y����
          */
         inline void SetY(double y) { _y = y; }
         /**
          * \brief �x�N�g���̒����̎擾
          * \return �x�N�g���̒���
          */
         const double Lenght() const;
         /**
          * \brief ���K�����s��
          */
         void Normalized();
         /**
          * \brief ���K�������l��߂�l�Ƃ��ĕԂ�
          * \return ���K�������l
          */
         const Vector4 Normalize() const;
         /**
          * \brief ���ς̎擾
          * \param rhs �擾����x�N�g��
          * \return ����
          */
         const double Dot(const Vector4& rhs) const;
         /**
          * \brief �O�ς̎擾
          * \param rhs �擾����x�N�g��
          * \return �O��
          */
         const Vector4 Cross(const Vector4& rhs) const;

      private:
         double _x;    //!< x����
         double _y;    //!< y����
         double _z;    //!< z����
         double _w;    //!< w����
      };
   }
}
