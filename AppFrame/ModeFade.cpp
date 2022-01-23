
/*****************************************************************//**
 * \file   ModeFade.cpp
 * \brief  ���[�h�̃t�F�[�h�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <DxLib.h>
#include "ModeFadeBase.h"
#include "ModeFadeIn.h"
#include "ModeFadeOut.h"
#include "ModeServer.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W���O���
    */
   namespace Mode {
      namespace {
         constexpr auto AlphaMin = 0.f;       //!< �����x�̍ŏ��l
         constexpr auto AlphaMax = 255.f;     //!< �����x�̍ő�l
         constexpr auto ShortFadeTime = 30.f; //!< �t�F�[�h���ԁi���j
         constexpr auto MiddleFadeTime = 60.f;//!< �t�F�[�h���ԁi���j
         constexpr auto LongFadeTime = 90.f;  //!< �t�F�[�h���ԁi��j
      }

      ModeFadeBase::ModeFadeBase(Game::GameBase& appBase) : ModeBaseRoot(appBase),
         _color(GetColor(0, 0, 0)) {
      }

      ModeFadeBase::~ModeFadeBase() {
      }

      void ModeFadeBase::Init() {
         _color = GetColor(0, 0, 0);
      }

      void ModeFadeBase::Enter() {
      }

      void ModeFadeBase::Update() {
      }

      void ModeFadeBase::Render() {
         SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(_alpha));
         DrawBox(0, 0, _width, _height, _color, TRUE);
         SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
      }

      ModeFadeIn::ModeFadeIn(Game::GameBase& gameBase) : ModeFadeBase(gameBase) {
      }

      void ModeFadeIn::Enter() {
         //���������x�̐ݒ�i�t�F�[�h�C�����Ă������߁A���摜�̓����xMAX�ŏ������j
         _alpha = AlphaMax;
         //�t�F�[�h�^�C�v�̕������Ƀt�F�[�h���Ԃ̐ݒ���\�ɂ���
         if (_fadeType == 'S') {
            _deltaAlpha = -(AlphaMax / ShortFadeTime);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = -(AlphaMax / MiddleFadeTime);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = -(AlphaMax / LongFadeTime);
         }
      }
      void ModeFadeIn::Update() {
         //�����x�����X�Ɍ��������A�ŏ��l�ɒB�����Ȃ炱�̃��[�h���|�b�v�o�b�N
         _alpha += _deltaAlpha;
         if (_alpha <= AlphaMin) {
            _alpha = AlphaMin;
            GetModeServer().PopBack();
         }
      }

      ModeFadeOut::ModeFadeOut(Game::GameBase& gameBase) : ModeFadeBase(gameBase) {
      }

      void ModeFadeOut::Enter() {
         //���������x�̐ݒ�i�t�F�[�h�A�E�g���Ă������߁A���摜�̓����x0�ŏ������j
         _alpha = AlphaMin;
         //�t�F�[�h�^�C�v�̕������Ƀt�F�[�h���Ԃ̐ݒ���\�ɂ���
         if (_fadeType == 'S') {
            _deltaAlpha = (AlphaMax / ShortFadeTime);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = (AlphaMax / MiddleFadeTime);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = (AlphaMax / LongFadeTime);
         }
      }

      void ModeFadeOut::Update() {
         //�����x�����X�ɑ��������A�ő�l�ɒB�����Ȃ炱�̃��[�h���|�b�v�o�b�N���A���̃V�[�����ꏏ�Ƀ|�b�v�o�b�N����
         _alpha += _deltaAlpha;
         if (_alpha >= AlphaMax) {
            _alpha = AlphaMax;
            GetModeServer().PopBack();
            GetModeServer().PopBack();
         }
      }

   }
}