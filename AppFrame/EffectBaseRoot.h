#pragma once
/*****************************************************************//**
 * \file   EffectBaseRoot.h
 * \brief  �G�t�F�N�g���N���X�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Vector4.h"
#include <string_view>
#include <EffekseerForDXLib.h>
#include <tuple>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame{
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   namespace Input {
      class InputManager;
   }
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
     /**
      * \class �G�t�F�N�g���N���X�̊��N���X
      * \brief �Q�[���{�̑��ł��̃N���X���p�����ăG�t�F�N�g���N���X���`����
      */
      class EffectBaseRoot {
      public:
         /**
          * \brief �G�t�F�N�g�̏�ԗ�
          */
         enum class EffectState {
            Active,     //!< ����
            Paused,     //!< ��~
            Dead        //!< ���S
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�̎Q��
          */
         EffectBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~EffectBaseRoot();
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
          * \brief ���S�����Ԃ�
          * \return ���S��ԂȂ�true,�łȂ��Ȃ�false
          */
         inline bool IsDead() { return (_efcState == EffectState::Dead); }
         /**
          * \brief ���������Ԃ�
          * \return ������ԂȂ�true,�łȂ��Ȃ�false
          */
         inline bool IsActive() { return (_efcState == EffectState::Active); }
         /**
          * \brief ��Ԃ����S�ɐݒ�
          */
         void SetDead() { _efcState = EffectState::Dead; }
         /**
          * \brief �G�t�F�N�g�n���h����ResourceServer�ɓo�^����Ă���G�t�F�N�g�n���h���ɐݒ肷��
          * \param key ResourceServer�̃G�t�F�N�g�n���h���Ɋ֘A�t���Ă���C�ӂ̕�����
          */
         void SetEffectLoadHandle(std::string_view key);
         void PlayEffect();
         bool IsPlaying();
         void StopEffect();
         inline Math::Vector4 position() { return _position; }
         void SetPosition(Math::Vector4 pos);
         inline Math::Vector4 rotation() { return _rotation; }
         void SetRotation(Math::Vector4 rot);
         inline Math::Vector4 scale() { return _scale; }
         void SetScale(Math::Vector4 sca);
         double GetSpeed();
         void SetSpeed(double speed);
         std::tuple<int, int, int, int> GetEfcColor() { return _color; }
         void SetEfcColor(std::tuple<int, int, int, int> color);

      protected:
         Game::GameBase& _gameBase;
         EffectState _efcState{ EffectState::Active };
         int _loadHandle{ -1 };
         int _playHandle{ -1 };
         Math::Vector4 _position{ 0,0,0 };
         Math::Vector4 _rotation{ 0,0,0 };
         Math::Vector4 _scale{ 0,0,0 };
         std::tuple<int, int, int, int> _color{255,255,255,255};
      };
   }
} 