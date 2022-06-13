#pragma once
/*****************************************************************//**
 * \file   PlayerCreator.h
 * \brief  �v���C���[�̐����Ǘ��N���X
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
 * \date   December 2021
 *********************************************************************/
#include "CreatorBase.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      /**
       * \class �v���C���[�̐����Ǘ��N���X
       * \brief �v���C���[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class PlayerCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
          PlayerCreator(Game::GameMain& gameMain);
      private:
         /**
          * \brief �J�����̈ʒu�A��Ԃƃv���C���[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
          * \return �v���C���[�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}