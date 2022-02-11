#pragma once
/*****************************************************************//**
 * \file   Triangle.h
 * \brief  �O�p�`�N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include <tuple>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �f�[�^�֌W
    */
   namespace Data {
      /**
       * \class �O�p�`�N���X
       * \brief �O�p�`�`��p�̃f�[�^
       */
      class Triangle {
         using Apex = std::pair<double, double>;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param firstPos �O�p�`�̒��_1
          * \param secondPos �O�p�`�̒��_2
          * \param thirdPos �O�p�`�̒��_3
          */
         Triangle(Apex firstPos, Apex secondPos, Apex thirdPos) :
            _firstPos{ firstPos }, _secondPos{ secondPos }, _thirdPos{ thirdPos }{
         };
         /**
          * \brief ��̃R���X�g���N�^
          */
         Triangle() {};
         /**
          * \brief �O�p�`�̃f�[�^�̐ݒ�
          * \param firstPos
          * \param secondPos
          * \param thirdPos
          */
         void SetTriangleParams(Apex firstPos, Apex secondPos, Apex thirdPos) {
            _firstPos = firstPos, _secondPos = secondPos, _thirdPos = thirdPos;
         }
         /**
          * \brief �O�p�`�̃f�[�^�̎擾
          * \return �O�p�`�̃f�[�^
          */
         std::tuple<Apex, Apex, Apex> GetTriangleParams() { return { _firstPos ,_secondPos,_thirdPos }; }

      private:
         Apex _firstPos{ 0.0,0.0 };   //!< �O�p�`�̒��_�f�[�^1
         Apex _secondPos{ 0.0,0.0 };  //!< �O�p�`�̒��_�f�[�^2
         Apex _thirdPos{ 0.0,0.0 };   //!< �O�p�`�̒��_�f�[�^3
      };

   }
}
