#pragma once
#include "CreatorBase.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      
      class PoorEnemyAlmightyCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         PoorEnemyAlmightyCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief �ߐڍU�������Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
          * \return �ߐڍU�������Ă���G���G�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}