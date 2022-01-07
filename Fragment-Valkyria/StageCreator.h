#pragma once
/*****************************************************************//**
 * \file   StageCreator.h
 * \brief  �X�e�[�W�̐����Ǘ��N���X
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
       * \class �X�e�[�W�̐����Ǘ��N���X
       * \brief �X�J�C�X�t�B�A�y�ђn�ʂ̐������s��
       */
      class StageCreator : public CreatorBase {
      public:
          
          StageCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief �X�J�C�X�t�B�A�y�ђn�ʂ̐������s��
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \return �X�e�[�W�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create();
      };
   }
}
