#pragma once
/*****************************************************************//**
 * \file   GatlingCreator.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

   namespace Create {

      class GatlingCreator : public CreatorBase {
         public:
          GatlingCreator(Game::GameMain& gameMain);
      private:
         /**
          * \brief �v���C���[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
          * \param game �Q�[���{�̃N���X�̎Q��
          * \return �v���C���[�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;

      };
   }
}
