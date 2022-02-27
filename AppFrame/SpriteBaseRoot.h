#pragma once
/*****************************************************************//**
 * \file   SpriteBaseRoot.h
 * \brief  �X�v���C�g���N���X�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
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
      class TextureComponent;
   }
   namespace Sprite {
      //��d�C���N���[�h�h�~
      class SpriteServer;
      
      class SpriteBaseRoot {
         using Vector4 = Math::Vector4;
         using Matrix44 = Math::Matrix44;
         using StateServer = State::StateServer;
      public:
         
         
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         SpriteBaseRoot(Game::GameBase& gameBase);
         

         
         /**
          * \brief �Q�[���x�[�X�̎Q�Ƃ̎擾
          * \return �Q�[���x�[�X�̎Q��
          */
         inline Game::GameBase& gameBase() const { return _gameBase; }
         

      protected:
         /**
          * \brief �I�u�W�F�N�g�̏�Ԃ��擾
          * \return ���
          */
         inline SpriteState objState() const { return _sprState; }
         /**
          * \brief �I�u�W�F�N�g�̏�Ԃ�ݒ�
          * \param state �ݒ肷����
          */
         inline void objState(SpriteState state) { _sprState = state; }

         Game::GameBase& _gameBase;                     //!< �Q�[���{�̃N���X�̎Q��
         std::unique_ptr<StateServer> _stateServer;     //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
         int _grHandle{ -1 };                           //!< �摜�n���h��
         int _cx{ 0 }, _cy{ 0 };                        //!< �摜�̊�_
         int _alpha{ 255 };                             //!< �����x
         double _scale{ 1.0 };                          //!< �g�嗦
         double _angle{ 0.0 };                          //!< �p�x
         bool _turnFlag{ false };                       //!< ���]����
         std::vector<int> _grHandles;                   //!< �摜�n���h���z��
         Vector4 _position{ 0,0,0 };                    //!< �ʒu
         SpriteState _sprState{ SpriteState::Active };  //!< �I�u�W�F�N�g�̏��
      };
   }
}