
/*****************************************************************//**
 * \file   SpriteServer.cpp
 * \brief  �X�v���C�g�T�[�o�[
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteServer.h"
#include <DxLib.h>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

namespace AppFrame {
   namespace Sprite {
      void SpriteServer::Add(std::unique_ptr<SpriteBaseRoot> spr) {
         spr->Init();
         if (_updating) {
            // �X�V���͕ۗ����̓��I�z��ɒǉ�����
            _pendingSprites.emplace_back(std::move(spr));
         }
         else {
            //�X�V���łȂ���Ώ������񂷗p�̓��I�z��ɒǉ�����
            _runSprites.emplace_back(std::move(spr));
         }
      }

      void SpriteServer::Input(AppFrame::Input::InputManager& input) {
         _updating = true;
         // _runSprites�ɓo�^����Ă���Active��Ԃ̃X�v���C�g�̓��͏�������
         for (auto&& spr : _runSprites) {
            if (spr->IsActive()) {
               spr->Input(input);
            }
         }
         _updating = false;
      }
      /// �X�V
      void SpriteServer::Update() {
         _updating = true;
         // _runSprites�ɓo�^����Ă���Active��Ԃ̃X�v���C�g�̍X�V��������
         for (auto&& spr : _runSprites) {
            if (spr->IsActive()) {
               spr->Update();
            }
         }
         _updating = false;

         // �ۗ����̃X�v���C�g��_runSprites�Ɉړ�����
         _runSprites.insert(_runSprites.end(),
            make_move_iterator(_pendingSprites.begin()),
            make_move_iterator(_pendingSprites.end()));

         // �ۗ����̃X�v���C�g���폜����
         _pendingSprites.clear();

         // ���񂾃X�v���C�g���폜����
         erase_if(_runSprites, [](auto&& act) { return act->IsDead(); });
      }

      void SpriteServer::Render() {
         for (auto&& spr : _runSprites) {
            // _runSprites�ɓo�^����Ă���Active��Ԃ̃X�v���C�g�̕`�揈������
            if (spr->IsActive()) {
               spr->Draw();
            }
         }
      }

      void SpriteServer::Clear() {
         // �S�Ă̓��I�z����̗v�f���폜����
         _runSprites.clear();
         _pendingSprites.clear();
      }
   }
}

