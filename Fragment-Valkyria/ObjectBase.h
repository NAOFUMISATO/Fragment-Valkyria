#pragma once
/*****************************************************************//**
 * \file   ObjectBase.h
 * \brief  �I�u�W�F�N�g�̊��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   namespace Model {
      class ModelAnimeComponent;
   }
   namespace Camera {
      class CameraComponent;
   }
   namespace Collision {
       class CollisionComponent;
   }
   /**
    * \brief �I�u�W�F�N�g�֌W
    */
   namespace Object {
      /**
       * \class �I�u�W�F�N�g�̊��N���X
       * \brief �e�I�u�W�F�N�g�͂��̃N���X���p�����Ē�`����
       */
      class ObjectBase :public AppFrame::Object::ObjectBaseRoot{
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief �I�u�W�F�N�g�̎�ʗ�
          */
         enum class ObjectType {
            Object = 0,   //!< �I�u�W�F�N�g
            Player,       //!< �v���C���[
            Enemy,        //!< �G
            BossStage,    //!< �{�X�X�e�[�W
            LargeEnemy,
            FallObject,
            Gatling
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ObjectBase(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         ~ObjectBase()override;
         /**
          * \brief ����������
          */
         void Init() override {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief �X�V����
          */
         void Update()override {};
         /**
          * \brief �`�揈��
          */
         void Draw() override {};

         /**
          * \brief �I�u�W�F�N�g�̎�ʂ�Ԃ��i�������z�֐����j
          * \return �h����Œ�`
          */
         virtual ObjectType GetObjType() const = 0;
         /**
          * \brief �Q�[���N���X�̎Q�Ƃ��擾
          * \return �Q�[���N���X�̎Q��
          */
         inline Game::GameMain& gameMain() const { return _gameMain; }
         /**
          * \brief �A�j���[�V�����ꊇ�Ǘ��N���X�̐ݒ�
          * \param model �A�j���[�V�����Ǘ��N���X�̃C���X�^���X
          */
         void modelAnimeComponent(std::unique_ptr<Model::ModelAnimeComponent> model);
         /**
          * \brief �J�����Ǘ��N���X�̐ݒ�
          * \param camera �J�����Ǘ��N���X�̃C���X�^���X
          */
         void cameraComponent(std::shared_ptr<Camera::CameraComponent> camera);
         /**
          * \brief �A�j���[�V�����ꊇ�Ǘ��N���X�̎擾
          * \return �A�j���[�V�����Ǘ��N���X
          */
         Model::ModelAnimeComponent& modelAnimeComponent() const { return *_modelAnimeComponent; }
         /**
          * \brief �J�����Ǘ��N���X�̎擾
          * \return �J�����Ǘ��N���X
          */
         Camera::CameraComponent& cameraComponent() const { return *_cameraComponent; }

         Collision::CollisionComponent& collisionComponent() { return *_collisionComponent; }

         void  collisionComponent(std::shared_ptr<Collision::CollisionComponent> collision) { _collisionComponent = collision; }

      protected:
         Game::GameMain& _gameMain;              //!< �Q�[���{�̃N���X�̎Q��
         std::unique_ptr<Model::ModelAnimeComponent> _modelAnimeComponent; //!< ���f���̃A�j���[�V�����Ǘ��N���X�̃|�C���^
         std::shared_ptr<Camera::CameraComponent> _cameraComponent;        //!< �J�����Ǘ��N���X�̃|�C���^
         std::shared_ptr<Collision::CollisionComponent> _collisionComponent;
      };
   }
}
