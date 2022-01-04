#include "EffectServer.h"

namespace AppFrame {
   namespace Effect {
      void EffectServer::Add(std::unique_ptr<EffectBaseRoot> efc) {
         if (_updating) {
            // �X�V���͕ۗ����̓��I�z��ɒǉ�����
            _pendingEffects.emplace_back(std::move(efc));
         }
         else {
            //�X�V���łȂ���Ώ������񂷗p�̓��I�z��ɒǉ�����
            _runEffects.emplace_back(std::move(efc));
         }
      }
      
      void EffectServer::Input(AppFrame::Input::InputManager& input){
         _updating = true;
         // _runEffects�ɓo�^����Ă���Active��Ԃ̃G�t�F�N�g�̓��͏�������
         for (auto&& efc : _runEffects) {
            if (efc->IsActive()) {
               efc->Input(input);
            }
         }
         _updating = false;
      }

      void EffectServer::Update() {
         _updating = true;
         // _runEffects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̍X�V��������
         for (auto&& efc : _runEffects) {
            if (efc->IsActive()) {
               efc->Update();
            }
         }
         _updating = false;

         // �ۗ����̃I�u�W�F�N�g��_runEffects�Ɉړ�����
         _runEffects.insert(_runEffects.end(),
            make_move_iterator(_pendingEffects.begin()),
            make_move_iterator(_pendingEffects.end()));

         // �ۗ����̃I�u�W�F�N�g���폜����
         _pendingEffects.clear();

         // ���񂾃A�N�^�[���폜����
         erase_if(_runEffects, [](auto&& act) { return act->IsDead(); });
      }

      void EffectServer::Render() {
         for (auto&& efc : _runEffects) {
            // _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̕`�揈������
            if (efc->IsActive()) {
               efc->Draw();
            }
         }
      }

      void EffectServer::Clear() {
         // �S�Ă̓��I�z����̗v�f���폜����
         _runEffects.clear();
         _pendingEffects.clear();
      }
   }
}
