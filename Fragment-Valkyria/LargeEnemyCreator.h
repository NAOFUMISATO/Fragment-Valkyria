#pragma once
/*****************************************************************//**
 * \file   LargeEnemyCreator.h
 * \brief  ���[�W�G�l�~�[�̐����Ǘ��N���X
 * 
 * \author AHMD2000
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
       * \class ���[�W�G�l�~�[�̐����Ǘ��N���X
       * \brief ���[�W�G�l�~�[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
       */
      class LargeEnemyCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         LargeEnemyCreator() {};
      private:
         /**
          * \brief ���[�W�G�l�~�[�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
          * \return ���[�W�G�l�~�[�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
      }
}
