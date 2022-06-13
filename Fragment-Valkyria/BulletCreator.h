#pragma once
/*****************************************************************//**
 * \file   BulletCreator.h
 * \brief  ���u��U���̐����Ǘ��N���X
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
       * \class ���u��U���̐����Ǘ��N���X
       * \brief ���u��U���̈ʒu�A��Ԃ̓o�^���s��
       */
      class BulletCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         BulletCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief ���u��U���̈ʒu�A��Ԃ̓o�^���s��
          * \return ���u��U���̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
