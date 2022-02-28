#pragma once
/*****************************************************************//**
 * \file   GameMain.h
 * \brief  �Q�[���{�̃N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   constexpr auto ScreenWidth = 1920;   //!< ����ʃT�C�Y
   constexpr auto ScreenHeight = 1080;  //!< �c��ʃT�C�Y
   constexpr auto ScreenDepth = 32;     //!< �r�b�g��
   // ��d�C���N���[�h�h�~
   namespace Create {
      class ObjectFactory;
   }
   namespace Object {
      class ObjectServer;
   }
   namespace Sprite {
      class SpriteServer;
   }
   namespace Stage {
      class LoadStageFromJson;
   }
   namespace Effect {
      class EffectServer;
   }
   /**
    * \brief �Q�[���{��
    */
   namespace Game {
      /**
       * \class �Q�[���{�̃N���X
       * \brief �Q�[���̃��C����������
       */
      class GameMain : public AppFrame::Game::GameBase {
         using GameBase = AppFrame::Game::GameBase;
      public:
         /**
          * \brief ����������
          * \param hInstance WinMain�̑�����
          * \return �������ɐ���������true�A���s������false��Ԃ�
          */
         bool Initialize(HINSTANCE hInstance)override;
         /**
          * \brief ���s
          */
         void Run()override;
         /**
          * \brief ��~
          */
         void ShutDown()override;
         /**
          * \brief ���͏���
          */
         void Input()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;

         /**
          * \brief �v�����Ԃ̎擾
          * \return �C���Q�[���Ōv�����Ă��鎞��
          */
         inline unsigned int ingameTimer() const { return _ingameTimer; }
         /**
          * \brief �v�����Ԃ̐ݒ�
          * \param timer �ݒ肷�鎞��
          */
         inline void ingameTimer(unsigned int timer) { _ingameTimer = timer; }
         /**
          * \brief �v�����Ԃ�i�߂�
          */
         inline void IngameTimerCount() { _ingameTimer++; }
         /**
          * \brief �Q�[�������x�̎擾
          * \return �Q�[�������x
          */
         inline std::tuple<double, double, int> sensitivity() const { return _sensitivity; }
         /**
          * \brief �Q�[�������x�̐ݒ�
          * \param cameraSens �J�������x
          * \param aimSens �G�C�����x
          * \param deadZone �f�b�h�]�[��
          */
         inline void sensitivity(double cameraSens,double aimSens,int deadZone) { 
            _sensitivity = std::make_tuple(cameraSens, aimSens, deadZone);
         }
         /**
          * \brief ��ʐݒ�̒l��Ԃ�
          * \return ��ʉ��T�C�Y�A��ʏc�T�C�Y�A��ʃr�b�g��
          */
         inline virtual std::tuple<int, int, int>GraphSize() { return { ScreenWidth,ScreenHeight,ScreenDepth }; }
         /**
          * \brief �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎擾
          * \return �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& objFactory() const { return *_objFactory; }
         /**
          * \brief �X�e�[�W���Ǘ��N���X�̎擾
          * \return �X�e�[�W���Ǘ��N���X�̎Q��
          */
         Stage::LoadStageFromJson& loadStage() const { return *_loadStage; }
         /**
          * \brief �I�u�W�F�N�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �I�u�W�F�N�g�T�[�o�[�̎Q��
          */
         inline Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief �X�v���C�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �X�v���C�g�T�[�o�[�̎Q��
          */
         inline Sprite::SpriteServer& sprServer() const { return *_sprServer; }
         /**
          * \brief �G�t�F�N�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �G�t�F�N�g�T�[�o�[�̃|�C���^
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }

      private:
         unsigned int _ingameTimer{ 0 };                        //!< �Q�[�����^�C�}�[
         std::tuple<double, double, int> _sensitivity;          //!< �Q�[�������x�y�уf�b�h�]�[���l��Tuple�^
         std::unique_ptr<Object::ObjectServer> _objServer;      //!< �I�u�W�F�N�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Sprite::SpriteServer> _sprServer;      //!< �X�v���C�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Effect::EffectServer> _efcServer;      //!< �G�t�F�N�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Create::ObjectFactory> _objFactory;    //!< �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̃��j�[�N�|�C���^
         std::unique_ptr<Stage::LoadStageFromJson> _loadStage;  //!< �X�e�[�W���Ǘ��N���X�̃��j�[�N�|�C���^
      };
   }
}
