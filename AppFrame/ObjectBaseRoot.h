#pragma once
#include <memory>
#include "Vector4.h"
#include "Matrix44.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   namespace Input {
      class InputManager;
   }
   namespace Resource {
      class LoadJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Effect {
      class EffectServer;
   }
   /**
    * \brief �I�u�W�F�N�g�֌W
    */
   namespace Object {
      //��d�C���N���[�h�h�~
      class ObjectServer;
      /**
       * \class �I�u�W�F�N�g�̊��N���X
       * \brief �e�I�u�W�F�N�g�͂��̃N���X���p�����Ē�`����
       */
      class ObjectBaseRoot {
         using Vector4 = Math::Vector4;
         using Matrix44 = Math::Matrix44;
         using StateServer = State::StateServer;
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
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ObjectBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~ObjectBaseRoot();
         /**
          * \brief ����������
          */
         virtual void Init() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief �X�V����
          */
         virtual void Update() {};
         /**
          * \brief �`�揈��
          */
         virtual void Draw() {};

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
          * \brief �Q�[���̎Q�Ƃ�Ԃ�
          * \return �Q�[���̎Q��
          */
         inline Game::GameBase& gameBase() const { return _gameBase; }
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
            vec.Normalized();
            Matrix44 matrix;
            matrix.RotateY(_rotation.GetY(), true);
            return vec * matrix;
         }

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
         Effect::EffectServer& GetEfcServer() const;
         /**
          * \brief json�t�@�C���Ǘ��N���X�̎Q�Ƃ��Q�[���{�̌o�R�Ŏ擾
          * \return json�t�@�C���Ǘ��N���X�̎Q��
          */
         AppFrame::Resource::LoadJson& GetLoadJson() const;

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
         /**
          * \brief �r���{�[�h�̕`����s��
          * \param pos �`��ʒu
          * \param size �`��T�C�Y(�������̃T�C�Y�A�c�����͏c����ɂ�茈��)
          * \param angle �`��p�x
          * \param handle �摜�n���h��
          */
         void DrawBillBoard(Vector4 pos,double size,double angle,std::vector<int> handle,double deltatime);

         ObjectState _objState{ ObjectState::Active }; //!< �I�u�W�F�N�g�̎�ʕϐ�
         std::unique_ptr<StateServer> _stateServer;    //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
         Game::GameBase& _gameBase;              //!< �Q�[���{�̃N���X�̎Q��
         Matrix44 _worldTransform{ Matrix44() }; //!< ���|���h�s��
         Vector4 _position{ 0,0,0 };             //!< �ʒu
         Vector4 _rotation{ 0,0,0 };             //!< ��]
         Vector4 _scale{ 1,1,1 };                //!< �g�嗦
         int _cnt{0};
      };
   }
}