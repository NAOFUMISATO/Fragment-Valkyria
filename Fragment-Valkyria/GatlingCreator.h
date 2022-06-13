#pragma once
/*****************************************************************//**
 * \file   GatlingCreator.h
 * \brief  �K�g�����O�U���̒e�̐����Ǘ��N���X
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
    * \brief  �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      /**
       * \class �K�g�����O�U���̒e�̐����Ǘ��N���X
       * \brief �K�g�����O�U���̒e�̈ʒu�A��Ԃ̓o�^���s��
       */
      class GatlingCreator : public CreatorBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         GatlingCreator() {};
      private:
         /**
          * \brief �K�g�����O�U���̒e�̈ʒu�A��Ԃ̓o�^���s��
          * \return �K�g�����O�U���̒e�̃C���X�^���X
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
