#pragma once
/*****************************************************************//**
 * \file   PoorEnemyAlmightyCreator.h
 * \brief  �S�Ă̍U�����s���Ă���G���G�̐����Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class �S�Ă̍U�����s���Ă���G���G�̐����N���X
       * \brief �S�Ă̍U�����s���Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class PoorEnemyAlmightyCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         PoorEnemyAlmightyCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief �S�Ă̍U�����s���Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
          * \return �S�Ă̍U�����s���Ă���G���G�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}