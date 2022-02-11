#pragma once
/*****************************************************************//**
 * \file   Rect.h
 * \brief  ��`�N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame{
   /**
    * \brief �f�[�^�֌W
    */
   namespace Data {
      /**
       * \class ��`�N���X
       * \brief ��`�`��p�̃f�[�^
       */
      class Rect {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param left ��`����X���W
          * \param top ��`����Y���W
          * \param right ��`�E��X���W
          * \param bottom ��`�E��Y���W
          */
         Rect(double left, double top,double right, double bottom) :
            _left(left),_top(top),_right(right),_bottom(bottom){
         };
         /**
          * \brief ��̃R���X�g���N�^
          */
         Rect() {};
         /**
          * \brief ��`�̃f�[�^�̐ݒ�
          * \param left ��`����X���W
          * \param top ��`����Y���W
          * \param right ��`�E��X���W
          * \param bottom ��`�E��Y���W
          */
         void SetRectParams(double left, double top, double right, double bottom){
            _left = left, _top = top, _right = right, _bottom = bottom;
         }
         /**
          * \brief ��`�̃f�[�^�̎擾
          * \return ��`�̃f�[�^
          */
         std::tuple<double, double, double, double> GetRectParams() { return {_left,_top,_right,_bottom}; }
         /**
          * \brief ��`�̉E��X���W�̐ݒ�
          * \param right ��`�̉E��X���W
          */
         void right(double right) { _right = right; }

      private:
         double _left{ 0 };   //!< ��`����X���W
         double _top{ 0 };    //!< ��`����Y���W
         double _right{ 0 };  //!< ��`�E��X���W
         double _bottom{ 0 }; //!< ��`�E��Y���W
      };

}
}
