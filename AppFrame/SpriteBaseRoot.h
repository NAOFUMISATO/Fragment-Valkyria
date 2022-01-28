#pragma once
#include <memory>
#include <vector>
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
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Texture {
      class SimpleTextureComponent;
   }
   namespace Sprite {
      //��d�C���N���[�h�h�~
      class SpriteServer;
      
      class SpriteBaseRoot {
         using Vector4 = Math::Vector4;
         using Matrix44 = Math::Matrix44;
         using StateServer = State::StateServer;
      public:
         
         enum class SpriteState {
            Active,       //!< ����
            Paused,       //!< ��~
            Dead          //!< ���S
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         SpriteBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~SpriteBaseRoot()=default;
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
         virtual void Draw();

         inline bool IsDead() { return (_sprState == SpriteState::Dead); }
         
         inline bool IsActive() { return (_sprState == SpriteState::Active); }
         
         inline void SetDead() { _sprState = SpriteState::Dead; }
         
         inline Game::GameBase& gameBase() const { return _gameBase; }

         
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         
         inline Vector4 position() const { return _position; }
         
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
         
         SpriteServer& GetSprServer() const;
         /**
          * \brief json�t�@�C���Ǘ��N���X�̎Q�Ƃ��Q�[���{�̌o�R�Ŏ擾
          * \return json�t�@�C���Ǘ��N���X�̎Q��
          */
         AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
         /**
          * \brief �摜�ȈՕ`��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return �摜�ȈՕ`��N���X�̎Q��
          */
         Texture::SimpleTextureComponent& GetSimpTexComponent() const;
         Resource::ResourceServer& GetResServer() const;

      protected:
        
         inline SpriteState objState() const { return _sprState; }
         
         inline void objState(SpriteState state) { _sprState = state; }

         SpriteState _sprState{ SpriteState::Active }; 
         std::unique_ptr<StateServer> _stateServer;    //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
         Game::GameBase& _gameBase;   //!< �Q�[���{�̃N���X�̎Q��
         Vector4 _position{ 0,0,0 };  //!< �ʒu
         std::vector<int> _grHandles;
         int _grHandle{ -1 };
         int _cx{ 0 }, _cy{ 0 };
         int _alpha{ 255 };
         double _scale{ 1.0 };
         double _angle{ 0.0 };
         bool _turnFlag{ false };
      };
   }
}