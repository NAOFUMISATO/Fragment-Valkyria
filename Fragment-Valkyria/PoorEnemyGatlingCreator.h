#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatlingCreator.h
 * \brief  �K�g�����O�U�������Ă���G���G�̐����Ǘ��N���X
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
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
       * \class �K�g�����O�U�������Ă���G���G�̐����Ǘ��N���X
       * \brief �K�g�����O�U�������Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class PoorEnemyGatlingCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         PoorEnemyGatlingCreator() {};

      private:
         /**
          * \brief �K�g�����O�U�������Ă���G���G�̃A�j���[�V�����A��Ԃ̓o�^���s��
          * \return �K�g�����O�U�������Ă���G���G�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
