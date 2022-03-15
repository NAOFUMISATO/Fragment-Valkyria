#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  �G�t�F�N�g�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
      /**
       * \class �G�t�F�N�g�̊��N���X
       * \brief �e�G�t�F�N�g�͂��̃N���X��h�����Ē�`����
       */
      class EffectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Color = AppFrame::Data::Color;
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
          * \brief �G�t�F�N�g�̎�ʗ�
          */
         enum class EffectType {
            Effect,             //!< �G�t�F�N�g
            GatlingBullet,      //!< �K�g�����O�e
            GatlingMuzzleFlash, //!< �K�g�����O�̔��Ή�(�}�Y���t���b�V��)
            ObjectFall,         //!< �I�u�W�F�N�g����
            Heal,               //!< ��
            WeakBullet,         //!< �v���C���[��U��
            BossBeam,           //!< �{�X�r�[���U��
            BossCharge,         //!< �{�X�r�[���`���[�W
            BossStan,           //!< �{�X�̃X�^��
            BossDieAfter,       //!< �{�X�̎��S��
            BossFall,           //!< �{�X����
            ObjectUp,           //!< �I�u�W�F�N�g�㏸
            PoorCrash,          //!< �G���G�̔j��
            BossCrash,          //!< �{�X�̔j��
            PreliminaryLight    //!< �\������
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectBase(Game::GameMain& gameMain,std::string_view key);
         /**
          * \brief ����������
          */
         virtual void Init() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(AppFrame::Input::InputManager& input) {};
         /**
          * \brief �X�V����
          */
         virtual void Update();
         /**
          * \brief �`�揈��
          */
         virtual void Draw() {};
         /**
          * \brief �G�t�F�N�g�̎�ʂ�Ԃ��i�������z�֐����j
          * \return �h����Œ�`
          */
         virtual EffectType GetEfcType() const = 0;
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
         inline Vector4 position() const { return _position; }
         /**
          * \brief �G�t�F�N�g�̈ʒu�̐ݒ�
          * \return �G�t�F�N�g��ݒ肷��ʒu
          */
         void position(Vector4 pos) { _position = pos; }
         /**
          * \brief �G�t�F�N�V�A�ɂ��ʒu�̐ݒ�
          */
         void SetPosition();
         /**
          * \brief �G�t�F�N�g�̉�]�p�̎擾
          * \return �G�t�F�N�g�̉�]�p
          */
         inline Vector4 rotation() const { return _rotation; }
         /**
          * \brief �G�t�F�N�g�̉�]�p�̐ݒ�
          * \param rot �G�t�F�N�g��ݒ肷���]�p
          */
         void rotation(Vector4 rot) { _rotation = rot; }
         /**
          * \brief �G�t�F�N�V�A�ɂ���]�p�̐ݒ�
          */
         void SetRotation();
         /**
          * \brief �G�t�F�N�g�̊g�嗦�̎擾
          * \return �G�t�F�N�g�̊g�嗦
          */
         inline Vector4 scale() const { return _scale; }
         /**
          * \brief �G�t�F�N�g�̊g�嗦�̐ݒ�
          * \param sca �G�t�F�N�g��ݒ肷��g�嗦
          */
         void scale(Vector4 sca) { _scale = sca; }
         /**
          * \brief �G�t�F�N�V�A�ɂ��g�嗦�̐ݒ�
          */
         void SetScale();
         /**
          * \brief �G�t�F�N�g�̍Đ����x�̎擾
          * \return �G�t�F�N�g�̍Đ����x
          */
         double speed()const { return _speed; }
         /**
          * \brief �G�t�F�N�g�̍Đ����x�̐ݒ�
          * \param speed �ݒ肷��Đ����x
          */
         void speed(double speed) { _speed = speed; }
         /**
          * \brief �G�t�F�N�V�A�ɂ��Đ����x�̐ݒ�
          */
         void SetSpeed();
         /**
          * \brief �G�t�F�N�g�̐F�̎擾
          * \return �G�t�F�N�g�̐F
          */
         inline AppFrame::Data::Color color() const { return _color; }
         /**
          * \brief �G�t�F�N�g�̐F�̐ݒ�
          * \param color �ݒ肷��F
          */
         void color(Color color) { _color = color; }
         /**
          * \brief �G�t�F�N�V�A�ɂ��F�̐ݒ�
          */
         void SetEfcColor();

      protected:
         Game::GameMain& _gameMain;                    //!< �Q�[���{�̃N���X�̎Q��
         EffectState _efcState{ EffectState::Active }; //!< �G�t�F�N�g�̏��
         int _loadHandle{ -1 };                        //!< �G�t�F�N�g��ǂݍ��ރn���h��
         int _playHandle{ -1 };                        //!< �G�t�F�N�g���Đ�����n���h��
         double _speed{ 10.0 };                        //!< �Đ����x
         Vector4 _position{ 0,0,0 };                   //!< �ʒu
         Vector4 _rotation{ 0,0,0 };                   //!< ��]�p
         Vector4 _scale{ 1.0,1.0,1.0 };                //!< �g�嗦
         Color _color{ Color() };                      //!< �F(RGBA)
      };
}
}
