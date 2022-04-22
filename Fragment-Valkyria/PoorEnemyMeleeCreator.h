#pragma once
/*****************************************************************//**
 * \file   PoorEnemyMeleeCreator.h
 * \brief  �ߐڍU�������Ă���G���G�̐����Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class �ߐڍU�������Ă���G���G�̐����Ǘ��N���X
       * \brief �ߐڍU�������Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class PoorEnemyMeleeCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         PoorEnemyMeleeCreator() {};

      private:
         /**
          * \brief �ߐڍU�������Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
          * \return �ߐڍU�������Ă���G���G�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}