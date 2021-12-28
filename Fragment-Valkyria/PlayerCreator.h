#pragma once
/*****************************************************************//**
 * \file   PlayerCreator.h
 * \brief  �v���C���[�̐����Ǘ��N���X
 *
 * \author NAOFUMISATO
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
      class PlayerCreator :public CreatorBase {
         /**
          * \brief �v���C���[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
          * \param game �Q�[���{�̃N���X�̎Q��
          * \return �v���C���[�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create(Game::GameMain& game);
      };
   }
}