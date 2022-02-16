#pragma once
/*****************************************************************//**
 * \file   Stage.h
 * \brief  �X�e�[�W�N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include <memory>
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Model {
      class ModelComponent;
   }
   /**
    * \brief �X�e�[�W�֌W
    */
   namespace Stage {
      // ��d�C���N���[�h�h�~
      class StageModelComponent;
      /**
       * \class �X�e�[�W�N���X
       * \brief �X�J�C�X�t�B�A�y�уX�e�[�W�̊Ǘ����s��
       */
      class Stage :public Object::ObjectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̃N���X�̎Q��
          */
         Stage(Game::GameMain& game);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Draw();
         /**
          * \brief �I�u�W�F�N�g�̎�ʂ�Ԃ�
          * \return �X�e�[�W
          */
         ObjectType GetObjType() const override { return ObjectType::Stage; }
         StageModelComponent& stageComponent() { return *_stageComponent; }

      private:
         std::unique_ptr<StageModelComponent> _stageComponent;       //!< �X�e�[�W�̃|�C���^
      };
   }
}
