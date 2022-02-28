#pragma once
/*****************************************************************//**
 * \file   FallObjectCreator.h
 * \brief  �����I�u�W�F�N�g�̐����Ǘ��N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"
#include "FallObject.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      /**
       * \class �����I�u�W�F�N�g�̐����Ǘ��N���X
       * \brief �����I�u�W�F�N�g�̃A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class FallObjectCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         FallObjectCreator(Game::GameMain& gameMain);
      private:
         /**
         * \brief �����I�u�W�F�N�g�̃A�j���[�V�����A��Ԃ̓o�^���s��
         * \return �����I�u�W�F�N�g�̃C���X�^���X
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
