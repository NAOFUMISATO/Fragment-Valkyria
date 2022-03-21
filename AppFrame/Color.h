#pragma once
/*****************************************************************//**
 * \file   Color.h
 * \brief  �J���[�N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �e�f�[�^�֌W
    */
   namespace Data {
      /**
       * \class �J���[�N���X
       * \brief �J���[�l�̊Ǘ��A�J���[�R�[�h�̎擾���s��
       */
      class Color {
      public:
         /**
          * \brief �R���X�g���N�^(�����l�܂�)
          * \param r �Ԓl
          * \param g �Βl
          * \param b �l
          * \param a �����l
          */
         Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
            _red(r), _green(g), _blue(b), _alpha(a) {
         };
         /**
          * \brief �R���X�g���N�^(�����l����)
          * \param r �Ԓl
          * \param g �Βl
          * \param b �l
          */
         Color(unsigned char r, unsigned char g, unsigned char b) :
            _red(r), _green(g), _blue(b) {
         };
         /**
          * \brief ��̃R���X�g���N�^
          */
         Color() {};
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~Color() = default;

         /**
          * \brief �J���[�R�[�h�̎擾
          * \return �J���[�R�[�h
          */
         unsigned int GetColorCode();
         /**
          * \brief �J���[�l�̎擾(�����x�܂�)
          * \return �����x�܂ރJ���[�l
          */
         std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> GetColorA() const { 
            return std::make_tuple(_red, _green, _blue, _alpha); 
         }
         /**
          * \brief �J���[�l�̎擾(�����x����)
          * \return �����x�����J���[�l
          */
         std::tuple<unsigned char, unsigned char, unsigned char> GetColor() const {
            return std::make_tuple(_red, _green, _blue);
         }
         /**
          * \brief �J���[�l�̐ݒ�(�����x�܂�)
          * \param r �Ԓl
          * \param g �Βl
          * \param b �l
          * \param a �����l
          */
         void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
            _red = r;_green = g;_blue = b;_alpha =a;
         }
         /**
          * \brief �J���[�l�̐ݒ�(�����x����)
          * \param r �Ԓl
          * \param g �Βl
          * \param b �l
          */
         void SetColor(unsigned char r, unsigned char g, unsigned char b) {
            _red = r; _green = g; _blue = b;
         }
         /**
          * \brief �Ԓl�̎擾
          * \return �Ԓl
          */
         unsigned char red() const { return _red; }
         /**
          * \brief �Ԓl�̐ݒ�
          * \param r �ݒ肷��Ԓl
          */
         void red(unsigned char r) { _red = r; }
         /**
          * \brief �Βl�̎擾
          * \return �Βl
          */
         unsigned char green() const { return _green; }
         /**
          * \brief �Βl�̐ݒ�
          * \param g �ݒ肷��Βl
          */
         void green(unsigned char g) { _green = g; }
         /**
          * \brief �l�̎擾
          * \return �l
          */
         unsigned char blue() const { return _blue; }
         /**
          * \brief �l�̐ݒ�
          * \param b �ݒ肷��l
          */
         void blue(unsigned char b) { _blue = b; }
         /**
          * \brief �����x�̎擾
          * \return �����x
          */
         unsigned char alpha() const { return _alpha; }
         /**
          * \brief �����x�̐ݒ�
          * \param a �ݒ肷�铧���x
          */
         void alpha(unsigned char a) { _alpha = a; }

      private:
         unsigned char _red{ 255 };    //!< �Ԓl
         unsigned char _green{ 255 };  //!< �Βl
         unsigned char _blue{ 255 };   //!< �l
         unsigned char _alpha{ 255 };  //!< �����x

      };
   }
}
