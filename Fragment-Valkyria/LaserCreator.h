#pragma once
/*****************************************************************//**
 * \file   LaserCreator.h
 * \brief  ���[�U�[�����Ǘ��N���X
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"
/**
 * \brief  �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief  �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      /**
       * \class ���[�U�[�����Ǘ��N���X
       * \brief ���[�U�[�̈ʒu�A��Ԃ̓o�^���s��
       */
      class LaserCreator : public CreatorBase
      {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         LaserCreator(Game::GameMain& gameMain);
      private:
         /**
         * \brief ���[�U�[�̈ʒu�A��Ԃ̓o�^���s��
         * \return ���[�U�[�̃C���X�^���X
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
