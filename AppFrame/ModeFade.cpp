
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
         constexpr auto ALPHA_MIN = 0.f;       //!< �����x�̍ŏ��l
         constexpr auto ALPHA_MAX = 255.f;     //!< �����x�̍ő�l
         constexpr auto SHORTFADE_TIME = 30.f; //!< �t�F�[�h���ԁi���j
         constexpr auto MIDDLEFADE_TIME = 60.f;//!< �t�F�[�h���ԁi���j
         constexpr auto LONGFADE_TIME = 90.f;  //!< �t�F�[�h���ԁi��j
      }

      ModeFadeBase::ModeFadeBase(Game::GameBase& appBase) : ModeBase(appBase),
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

      ModeFadeIn::ModeFadeIn(Game::GameBase& appBase) : ModeFadeBase(appBase) {
      }

      void ModeFadeIn::Enter() {
         //���������x�̐ݒ�i�t�F�[�h�C�����Ă������߁A���摜�̓����xMAX�ŏ������j
         _alpha = ALPHA_MAX;
         //�t�F�[�h�^�C�v�̕������Ƀt�F�[�h���Ԃ̐ݒ���\�ɂ���
         if (_fadeType == 'S') {
            _deltaAlpha = -(ALPHA_MAX / SHORTFADE_TIME);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = -(ALPHA_MAX / MIDDLEFADE_TIME);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = -(ALPHA_MAX / LONGFADE_TIME);
         }
      }
      void ModeFadeIn::Update() {
         //�����x�����X�Ɍ��������A�ŏ��l�ɒB�����Ȃ炱�̃��[�h���|�b�v�o�b�N
         _alpha += _deltaAlpha;
         if (_alpha <= ALPHA_MIN) {
            _alpha = ALPHA_MIN;
            GetModeServer().PopBack();
         }
      }

      ModeFadeOut::ModeFadeOut(Game::GameBase& appBase) : ModeFadeBase(appBase) {
      }

      void ModeFadeOut::Enter() {
         //���������x�̐ݒ�i�t�F�[�h�A�E�g���Ă������߁A���摜�̓����x0�ŏ������j
         _alpha = ALPHA_MIN;
         //�t�F�[�h�^�C�v�̕������Ƀt�F�[�h���Ԃ̐ݒ���\�ɂ���
         if (_fadeType == 'S') {
            _deltaAlpha = (ALPHA_MAX / SHORTFADE_TIME);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = (ALPHA_MAX / MIDDLEFADE_TIME);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = (ALPHA_MAX / LONGFADE_TIME);
         }
      }

      void ModeFadeOut::Update() {
         //�����x�����X�ɑ��������A�ő�l�ɒB�����Ȃ炱�̃��[�h���|�b�v�o�b�N���A���̃V�[�����ꏏ�Ƀ|�b�v�o�b�N����
         _alpha += _deltaAlpha;
         if (_alpha >= ALPHA_MAX) {
            _alpha = ALPHA_MAX;
            GetModeServer().PopBack();
            GetModeServer().PopBack();
         }
      }

   }
}