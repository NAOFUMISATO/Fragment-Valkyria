#pragma once
/*****************************************************************//**
 * \file   EffectBaseRoot.h
 * \brief  �G�t�F�N�g���N���X�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Vector4.h"
#include "Color.h"
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
          * \brief �������y��_loadHandle�ɃG�t�F�N�g�n���h����o�^����
          * \param key ResourceServer�ɓo�^�ς݂̔C�ӂ̕�����
          */
         virtual void Init(std::string_view key);
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
         /**
          * \brief �G�t�F�N�g���Đ�����
          */
         void PlayEffect();
         /**
          * \brief �G�t�F�N�g���Đ������̔���
          * \return �Đ����Ȃ�true,�łȂ����false
          */
         bool IsPlaying();
         /**
          * \brief �G�t�F�N�g���~����
          */
         void StopEffect();
         /**
          * \brief �G�t�F�N�g�̈ʒu�̎擾
          * \return �G�t�F�N�g�̈ʒu
          */
         inline Math::Vector4 position() { return _position; }
         /**
          * \brief �G�t�F�N�g�̈ʒu�̐ݒ�
          * \param pos �G�t�F�N�g��ݒ肷��ʒu
          */
         void SetPosition(Math::Vector4 pos);
         /**
          * \brief �G�t�F�N�g�̉�]�p�̎擾
          * \return �G�t�F�N�g�̉�]�p
          */
         inline Math::Vector4 rotation() { return _rotation; }
         /**
          * \brief �G�t�F�N�g�̉�]�p�̐ݒ�
          * \param rot �G�t�F�N�g��ݒ肷���]�p
          */
         void SetRotation(Math::Vector4 rot);
         /**
          * \brief �G�t�F�N�g�̊g�嗦�̎擾
          * \return �G�t�F�N�g�̊g�嗦
          */
         inline Math::Vector4 scale() { return _scale; }
         /**
          * \brief �G�t�F�N�g�̊g�嗦�̐ݒ�
          * \param sca �G�t�F�N�g��ݒ肷��g�嗦
          */
         void SetScale(Math::Vector4 sca);
         /**
          * \brief �G�t�F�N�g�̍Đ����x�̎擾
          * \return �G�t�F�N�g�̍Đ����x
          */
         double GetSpeed();
         /**
          * \brief �G�t�F�N�g�̍Đ����x�̐ݒ�
          * \param speed �G�t�F�N�g��ݒ肷��Đ����x
          */
         void SetSpeed(double speed);
         /**
          * \brief �G�t�F�N�g�̐F�̎擾
          * \return �G�t�F�N�g�̐F
          */
         inline Color::Color color() { return _color; }
         /**
          * \brief �G�t�F�N�g�̐F(RGBA)��ݒ�
          * \param color �G�t�F�N�g��ݒ肷��F(RGBA)
          */
         void SetEfcColor(Color::Color _color );

      protected:
         Game::GameBase& _gameBase;                    //!< �Q�[���x�[�X�̎Q��
         EffectState _efcState{ EffectState::Active }; //!< �G�t�F�N�g�̏��
         int _loadHandle{ -1 };                        //!< �G�t�F�N�g��ǂݍ��ރn���h��
         int _playHandle{ -1 };                        //!< �G�t�F�N�g���Đ�����n���h��
         double _speed{ 10.0 };                        //!< �Đ����x
         Math::Vector4 _position{ 0,0,0 };             //!< �ʒu
         Math::Vector4 _rotation{ 0,0,0 };             //!< ��]�p
         Math::Vector4 _scale{ 1.0,1.0,1.0 };          //!< �g�嗦
         Color::Color _color{ Color::Color() };        //!< �F(RGBA)
      };
   }
} 