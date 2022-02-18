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
      class ObjectServer;
      /**
       * \class �I�u�W�F�N�g�̊��N���X
       * \brief �e�I�u�W�F�N�g�͂��̃N���X���p�����Ē�`����
       */
      class ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief �I�u�W�F�N�g�̏�ԗ�
          */
         enum class ObjectState {
            Active,       //!< ����
            Paused,       //!< ��~
            Dead          //!< ���S
         };

         /**
          * \brief �I�u�W�F�N�g�̎�ʗ�
          */
         enum class ObjectType {
            Object = 0,       //!< �I�u�W�F�N�g
            Player,           //!< �v���C���[
            Enemy,            //!< �G
            Stage,            //!< �X�e�[�W
            LargeEnemy,       
            FallObject,
            Gatling,
            Bullet,
            Laser,
            PoorEnemyGatling
         };

         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ObjectBase(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~ObjectBase();
         /**
          * \brief ����������
          */
         virtual void Init() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(AppFrame::Input::InputManager& input){};
         /**
          * \brief �X�V����
          */
         virtual void Update() {};
         /**
          * \brief �`�揈��
          */
         virtual void Draw() {};
         /**
          * \brief �I�u�W�F�N�g�̎�ʂ�Ԃ��i�������z�֐����j
          * \return �h����Œ�`
          */
         virtual ObjectType GetObjType() const = 0;

         /**
         * \brief �I�u�W�F�N�g�̎��S�����Ԃ�
         * \return �I�u�W�F�N�g�̏�Ԃ�Dead�Ȃ��True�A�łȂ����false��Ԃ�
         */
         inline bool IsDead() { return (_objState == ObjectState::Dead); }
         /**
          * \brief �I�u�W�F�N�g�̐��������Ԃ�
          * \return �I�u�W�F�N�g�̏�Ԃ�Active�Ȃ��True�A�łȂ����false��Ԃ�
          */
         inline bool IsActive() { return (_objState == ObjectState::Active); }
         /**
          * \brief �I�u�W�F�N�g�����S��Ԃɐݒ�
          */
         inline void SetDead() { _objState = ObjectState::Dead; }
         /**
          * \brief ���[���h�s��̐ݒ�
          */
         virtual void ComputeWorldTransform();
         /**
          * \brief ���[���h�s��̎擾
          * \return ���[���h�s��
          */
         inline Matrix44& worldTransform() { return _worldTransform; }
         /**
          * \brief �I�u�W�F�N�g�̌����̎擾
          * \return �I�u�W�F�N�g�̉�]�x�N�g��
          */
         Vector4 GetForward() const {
            auto vec = Vector4{ 0,0,1 };
            auto matrix = Matrix44();
            matrix.RotateY(_rotation.GetY(), false);
            auto forwardVec = vec * matrix;
            forwardVec.Normalized();
            return forwardVec;
         }
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
         const std::shared_ptr<Camera::CameraComponent>& cameraComponent() const { return _cameraComponent; }
         
         Collision::CollisionComponent& collisionComponent() { return *_collisionComponent; }

         void  collisionComponent(std::shared_ptr<Collision::CollisionComponent> collision) { _collisionComponent = collision; }
         /**
          * \brief �I�u�W�F�N�g�ʒu�̐ݒ�
          * \param pos �I�u�W�F�N�g�ʒu
          */
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         /**
          * \brief �I�u�W�F�N�g�ʒu���擾
          * \return �I�u�W�F�N�g�ʒu
          */
         inline Vector4 position() const { return _position; }
         /**
          * \brief �I�u�W�F�N�g�̉�]�p�̐ݒ�
          * \param rot �I�u�W�F�N�g�̉�]
          */
         inline void rotation(const Vector4& objRotation) { _rotation = objRotation; }
         /**
          * \brief �I�u�W�F�N�g�̉�]�p���擾
          * \return �I�u�W�F�N�g�̉�]
          */
         inline Vector4 rotation() const { return _rotation; }
         /**
          * \brief �I�u�W�F�N�g�̊g�嗦�̐ݒ�
          * \param sca �I�u�W�F�N�g�̊g�嗦
          */
         inline void scale(const Vector4& objScale) { _scale = objScale; }
         /**
          * \brief �I�u�W�F�N�g�̊g�嗦�̎擾
          * \return �I�u�W�F�N�g�̊g�嗦
          */
         inline Vector4 scale() const { return _scale; }
         /**
          * \brief ��Ԉꊇ�Ǘ��N���X�̐ݒ�
          * \param state �e��ԃN���X�̃C���X�^���X
          */
         void stateServer(std::unique_ptr<StateServer> state);
         /**
          * \brief ��Ԉꊇ�Ǘ��N���X�̎擾
          * \return ��ԊǗ��N���X
          */
         inline StateServer& stateServer() const { return *_stateServer; }
         /**
          * \brief �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���{�̂��o�R���擾
          * \return �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q��
          */
         ObjectServer& GetObjServer() const;
         /**
          * \brief �G�t�F�N�g�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���{�̌o�R�Ŏ擾
          * \return �G�t�F�N�g�ꊇ�Ǘ��N���X�̎Q��
          */
         AppFrame::Effect::EffectServer& GetEfcServer() const;
         /**
          * \brief json�t�@�C���Ǘ��N���X�̎Q�Ƃ��Q�[���{�̌o�R�Ŏ擾
          * \return json�t�@�C���Ǘ��N���X�̎Q��
          */
         AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
         /**
          * \brief �摜�`��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return �摜�`��N���X�̎Q��
          */
         AppFrame::Texture::TextureComponent& GetTexComponent() const;
         /**
          * \brief �T�E���h�R���|�[�l���g�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return �T�E���h�R���|�[�l���g�̎Q��
          */
         AppFrame::Sound::SoundComponent& GetSoundComponent() const;

      protected:
         /**
          * \brief �I�u�W�F�N�g�̏�Ԃ��擾
          * \return �I�u�W�F�N�g�̏��
          */
         inline ObjectState objState() const { return _objState; }
         /**
          * \brief �I�u�W�F�N�g�̏�Ԃ�ݒ�
          * \param state �I�u�W�F�N�g�̏��
          */
         inline void objState(ObjectState state) { _objState = state; }

         ObjectState _objState{ ObjectState::Active };                       //!< �I�u�W�F�N�g�̎�ʕϐ�
         std::unique_ptr<StateServer> _stateServer;                          //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
         Matrix44 _worldTransform{ Matrix44() };                             //!< ���|���h�s��
         Vector4 _position{ 0,0,0 };                                         //!< �ʒu
         Vector4 _rotation{ 0,0,0 };                                         //!< ��]
         Vector4 _scale{ 1,1,1 };                                            //!< �g�嗦
         Game::GameMain& _gameMain;                                          //!< �Q�[���{�̃N���X�̎Q��
         std::unique_ptr<Model::ModelAnimeComponent> _modelAnimeComponent;   //!< ���f���̃A�j���[�V�����Ǘ��N���X�̃|�C���^
         std::shared_ptr<Camera::CameraComponent> _cameraComponent;          //!< �J�����Ǘ��N���X�̃|�C���^
         std::shared_ptr<Collision::CollisionComponent> _collisionComponent; //!< 
      };
   }
}
