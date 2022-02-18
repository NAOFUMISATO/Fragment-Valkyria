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
      class LoadResourceJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Effect {
      class EffectServer;
   }
   namespace Texture {
      class TextureComponent;
   }
   namespace Sound {
      class SoundComponent;
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
         ;
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

        

         

      protected:
         

         
      };
   }
}