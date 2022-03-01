#pragma once
/*****************************************************************//**
 * \file   SpriteBase.h
 * \brief  �X�v���C�g�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
   /**
    * \brief �X�v���C�g�֌W
    */
   namespace Sprite {
      // ��d�C���N���[�h�h�~
      class SpriteServer;
      /**
       * \class �X�v���C�g�̊��N���X
       * \brief �e�X�v���C�g�͂��̃N���X��h�����Ē�`����
       */
      class SpriteBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief �X�v���C�g�̏�ԗ�
          */
         enum class SpriteState {
            Active,       //!< ����
            Paused,       //!< ��~
            Dead          //!< ���S
         };
         /**
          * \brief �X�v���C�g�̎�ʗ�
          */
         enum class SpriteType {
            Sprite = 0,      //!< �X�v���C�g
            PlayerHP,        //!< �v���C���[HP
            LargeEnemyHP,    //!< �{�XHP
            RemainingBullet, //!< �c���U���e��
            RemainingPortion,//!< �c��񕜃A�C�e����
            Reticle          //!< ���e�B�N��
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         SpriteBase(Game::GameMain& gameMain);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~SpriteBase() = default;
         /**
          * \brief ����������
          */
         virtual void Init() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(AppFrame::Input::InputManager & input) {};
         /**
          * \brief �X�V����
          */
         virtual void Update() {};
         /**
          * \brief �`�揈��
          */
         virtual void Draw();
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return �h����Œ�`
          */
         virtual SpriteType GetSprType() const = 0;
         /**
          * \brief �Q�[���{�̃N���X�̎Q�Ƃ��擾
          * \return �Q�[���{�̃N���X�̎Q��
          */
         inline Game::GameMain& gameMain() const { return _gameMain; }
         /**
          * \brief ���S���Ă��邩�̔���
          * \return ���S���Ă����true�A�łȂ����false��Ԃ�
          */
         inline bool IsDead() { return (_sprState == SpriteState::Dead); }
         /**
          * \brief �������Ă��邩�̔���
          * \return �������Ă����true�A�łȂ����false��Ԃ�
          */
         inline bool IsActive() { return (_sprState == SpriteState::Active); }
         /**
          * \brief ��Ԃ����S��Ԃɐݒ�
          */
         inline void SetDead() { _sprState = SpriteState::Dead; }
         /**
          * \brief �ʒu�̐ݒ�
          * \param objPosition �ʒu
          */
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         /**
          * \brief �ʒu�̎擾
          * \return �ʒu
          */
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
         /**
          * \brief �X�v���C�g�T�[�o�[�̎Q�Ƃ��Q�[���{�̃N���X�o�R�Ŏ擾
          * \return �X�v���C�g�T�[�o�[�̎Q��
          */
         SpriteServer& GetSprServer() const;
         /**
          * \brief json�t�@�C���Ǘ��N���X�̎Q�Ƃ��Q�[���{�̃N���X�o�R�Ŏ擾
          * \return json�t�@�C���Ǘ��N���X�̎Q��
          */
         AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
         /**
          * \brief �摜�ȈՕ`��N���X�̎Q�Ƃ��Q�[���{�̃N���X�o�R�Ŏ擾
          * \return �摜�ȈՕ`��N���X�̎Q��
          */
         AppFrame::Texture::TextureComponent& GetTexComponent() const;
         /**
          * \brief ���\�[�X�T�[�o�[�̎Q�Ƃ��Q�[���{�̃N���X�o�R�Ŏ擾
          * \return ���\�[�X�T�[�o�[�̎Q��
          */
         AppFrame::Resource::ResourceServer& GetResServer() const;

      protected:
         /**
         * \brief �X�v���C�g�̏�Ԃ��擾
         * \return �X�v���C�g�̏��
         */
         inline SpriteState sprState() const { return _sprState; }
         /**
          * \brief �X�v���C�g�̏�Ԃ�ݒ�
          * \param state �ݒ肷��X�v���C�g�̏��
          */
         inline void sprState(SpriteState state) { _sprState = state; }

         Game::GameMain& _gameMain;                     //!< �Q�[���{�̃N���X�̎Q��
         int _grHandle{ -1 };                           //!< �摜�n���h��
         int _cx{ 0 };                                  //!< �摜��X��_
         int _cy{ 0 };                                  //!< �摜��Y��_
         int _alpha{ 255 };                             //!< �����x
         bool _transFlag{ true };                       //!< �w�i���߃t���O
         bool _turnFlag{ false };                       //!< ���]����
         double _scale{ 1.0 };                          //!< �g�嗦
         double _angle{ 0.0 };                          //!< �p�x
         Vector4 _position{ 0,0,0 };                    //!< �ʒu
         std::vector<int> _grHandles;                   //!< �摜�n���h���z��
         SpriteState _sprState{ SpriteState::Active };  //!< �I�u�W�F�N�g�̏��
         std::unique_ptr<StateServer> _stateServer;     //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
      };
   }
}
