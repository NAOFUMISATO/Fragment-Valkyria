/*****************************************************************//**
 * \file   ResourceServver.cpp
 * \brief  ���\�[�X�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ResourceServer.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "GameBase.h"
#include "CurrentPathServer.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      ResourceServer::ResourceServer() {
         Init();
      }

      void ResourceServer::Init() {
         Release();        // ���\�[�X�̑S���
      }

      void ResourceServer::Release() {
         ClearTextures();  // �摜���̉��
         ClearModels();    // ���f�����̉��
         ClearSounds();    // �������̉��
         ClearEffects();   // �G�t�F�N�g���̉��
      }

      /*----------2D�֌W----------*/

      void ResourceServer::ClearTextures() {
         for (auto&& [key, divGraphAndHandles] : _textures) {
            auto&& [divGraph, handles] = divGraphAndHandles;
            // �摜���̃R���e�i��S�ĉ񂵁A�摜�̍폜���s��
            for (auto&& handle : handles) {
               DeleteGraph(handle);
            }
            // �摜�n���h���R���e�i�̉��
            handles.clear();
         }
         // �摜���̃R���e�i�̉��
         _textures.clear();
      }

      void ResourceServer::LoadTexture(std::string_view key, Texture& texture) {
#ifndef _DEBUG
         // �摜���R���e�i����w��̃L�[����������
         if (_textures.contains(key.data())) {
            auto&& [texture, handles] = _textures[key.data()];
            // �o�^�ς݂̏ꍇ�͉摜���폜
            for (auto handle : handles) {
               DeleteGraph(handle);
            }
            // �摜�n���h���R���e�i�̉��
            handles.clear();
            // �w�肵���L�[�̍폜
            _textures.erase(key.data());
         }
#else
         // �摜���R���e�i����w��̃L�[����������
         try {
            if (_textures.contains(key.data())) {
               auto&& [texture, handles] = _textures[key.data()];
               // �o�^�ς݂̏ꍇ�͉摜���폜
               for (auto handle : handles) {
                  DeleteGraph(handle);
               }
               // �摜�n���h���R���e�i�̉��
               handles.clear();
               // �w�肵���L�[�̍폜
               _textures.erase(key.data());
               // �摜�n���h���̃A�h���X���ς�邽�߁A�G���[���b�Z�[�W���o�͂���
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�̉摜���ēx�A�ǂݍ��܂�܂����B�ȑO�̉摜�n���h���f�[�^�͔j������܂��B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �e�摜���̎擾
         auto filename = texture.fileName().data();
         auto [allnum, xnum, ynum, xsize, ysize] = texture.GetDivParams();
         std::vector<int> handles(allnum);
         // �摜�𕪊��ǂݍ��݂���
         LoadDivGraph(filename, allnum, xnum, ynum, xsize, ysize, handles.data());
         // �摜���R���e�i�ɍ쐬�����n���h�����i�[����
         _textures.emplace(key, std::make_pair(texture, handles));
      }

      int ResourceServer::GetTexture(std::string_view key, int no) {
#ifndef _DEBUG
         // �摜���R���e�i�Ɏw��̃L�[��������΁A-1��Ԃ�
         if (!_textures.contains(key.data())) {
            return -1;
         }
#else
         // �摜���R���e�i�Ɏw��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[["+ message +"]���摜���R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto&& [divGraph, handles] = _textures[key.data()];
         // �摜���R���e�ino�Ԗڂ̗v�f�̎Q�Ƃ�Ԃ�
         auto handle = handles.at(no);
         // ��L�̃n���h����Ԃ�
         return handle;
      }

      std::vector<int> ResourceServer::GetTextures(std::string_view key) {
#ifndef _DEBUG
         // �摜���R���e�i�Ɏw��̃L�[��������΁A��̓��I�z���Ԃ�
         if (!_textures.contains(key.data())) {
            return std::vector<int>(0);
         }
#else
         // �摜���R���e�i�Ɏw��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���摜���R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �w��̃L�[�́A�n���h���Q��Ԃ�
         auto&& [divGraph, handles] = _textures[key.data()];
         return handles;
      }

      Texture ResourceServer::GetTextureInfo(std::string_view key) {
#ifndef _DEBUG
         // �摜���R���e�i�Ɏw��̃L�[��������΁A�摜���̋�̃N���X��Ԃ�
         if (!_textures.contains(key.data())) {
            return Texture();
         }
#else
         // �摜���R���e�i�Ɏw��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[["+ message +"]���摜���R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �w��̃L�[�̉摜���̃N���X��Ԃ�
         auto&& [divGraph, handles] = _textures[key.data()];
         return divGraph;
      }

      /*----------3D�֌W----------*/

      void ResourceServer::ClearModels() {
         for (auto&& [key, model] : _models) {
            auto&& [handles,animes] = model;
            // ���f�����̃R���e�i��S�ĉ񂵁A���f���̍폜���s��
            for (auto handle : handles) {
               MV1DeleteModel(handle);
            }
            // �摜�n���h���R���e�i�̉��
            handles.clear();
            // �A�j�����R���e�i�̉��
            animes.clear();
         }
         // ���f�����̃R���e�i�̉��
         _models.clear();
      }

      void ResourceServer::DeleteDuplicateModels() {
         for (auto&& [key, model] : _models) {
            // �I���W�i���̃n���h���͔z��̐擪�̂��߁A����ȊO�̕���(Duplicate)�������f���͍폜����
            auto&& [handles,animes] = model;
            // 0�Ԃ��I���W�i��
            auto original = handles[0];
            erase_if(handles, [original](auto handle) {
               // �I���W�i���ȊO�̃��f���n���h���͍폜����
               if (original != handle) {
                  MV1DeleteModel(handle);
                  return true;
               }
               return false;
               });
         }
      }

      int ResourceServer::LoadModel(std::string_view key, const std::string_view filename) {
#ifndef _DEBUG
         // �w��̃L�[������΁A���f�����폜
         if (_models.contains(key.data())) {
            auto& [handles,animes] = _models[key.data()];
            for (auto handle : handles) {
               MV1DeleteModel(handle);
            }
            // ���f���n���h���R���e�i�̉��
            handles.clear();
            // �A�j�����R���e�i�̉��
            animes.clear();
            // �w�肵���L�[�̍폜
            _models.erase(key.data());
         }
#else
         // �w��̃L�[������΁A���f�����폜
         try {
            if (_models.contains(key.data())) {
               auto& [handles, animes] = _models[key.data()];
               for (auto handle : handles) {
                  MV1DeleteModel(handle);
               }
               // ���f���n���h���R���e�i�̉��
               handles.clear();
               // �A�j�����R���e�i�̉��
               animes.clear();
               // �w�肵���L�[�̍폜
               _models.erase(key.data());
               // ���f���R���e�i�Ɏw��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�̃��f�����ēx�A�ǂݍ��܂�܂����B�ȑO�̃��f���n���h���f�[�^�͔j������܂��B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // ���f����ǂݍ��݁A�n���h���ɓo�^����
         auto handle = MV1LoadModel(filename.data());
         // ���f���n���h�����i�[���铮�I�z��̐擪�Ƀn���h����o�^����
         std::vector<int> handles{ handle };
         // �A�j���[�V�����������擾
         auto animNum = MV1GetAnimNum(handle);
         // �A�z�z��Ɏ擾�����A�j�������L�[�Ƃ����A�j���[�V������o�^����
         std::unordered_map<std::string, int> animes;
         for (int i = 0; i < animNum; ++i) {
            auto animName = MV1GetAnimName(handle, i);
            animes.emplace(animName, i);
         }
         // ���f�����R���e�i�ɍ쐬�����n���h���ƁA�A�j�����R���e�i��o�^����
         _models.emplace(key, std::make_pair(handles,animes));
         // ���f���n���h����Ԃ�
         return handle;
      }

      std::pair<int, int> ResourceServer::GetModel(std::string_view key, int no) {
#ifndef _DEBUG
         // �w��̃L�[��������΁A��̃f�[�^��Ԃ�
         if (!_models.contains(key.data())) {
            return std::make_pair(-1, no);
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_models.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[["+ message +"]�����f�����R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // ���f�����̎擾
         auto& [handles,animes] = _models[key.data()];
         // no������no�Ȃ�V�����o�^����
         if (no < handles.size()) {
            return std::make_pair(handles[no], no);
         }
         // �V�Kno�̏ꍇ�͕�������
         auto handle = MV1DuplicateModel(handles[0]);
         // ���I�z��ɕ����������f����o�^����
         handles.push_back(handle);
         // �w��no�̃n���h����Ԃ�
         return std::make_pair(handle, static_cast<int>(handles.size()) - 1);
      }

      int ResourceServer::GetModelAnimIndex(std::string_view key, std::string_view animName) {
#ifndef _DEBUG
         if (!_models.contains(key.data())) {
            return -1;   // �L�[�����o�^
         }
#else
         try {
            if (!_models.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�A�j���[�V�������������悤�Ƃ��܂������A�L�[[" + message + "]�����f�����R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif

         auto& [handles, animes] = _models[key.data()];

#ifndef _DEBUG
         if (!animes.contains(animName.data())) {
             return -1;   // �L�[�����o�^
         }
#else
         try {
             if (!animes.contains(animName.data())) {
                 std::string message = animName.data();
                 throw std::logic_error("----------�L�[[" + message + "]���A�j�����R���e�i�ɑ��݂��܂���ł����B----------\n");
             }
         }
         catch (std::logic_error& error) {
             OutputDebugString(error.what());
         }
#endif

         return animes[animName.data()];
      }

      /*-----------�����֌W----------*/


      void ResourceServer::ClearSounds() {
         for (auto&& [key, sound] : _sounds) {
            auto&& [soudData, handle] = sound;
            DeleteSoundMem(handle);
         }
         _sounds.clear();
      }

      void ResourceServer::LoadSound(std::string_view key, std::pair<SoundData, bool> soundDataAndIsLoad) {
#ifndef _DEBUG
         // �w��̃L�[�̌���
         if (_sounds.contains(key.data())) {
            auto&& [soudData, handle] = _sounds[key.data()];
            // �����n���h���̉��
            DeleteSoundMem(handle);
            // �w��̃L�[�̍폜
            _sounds.erase(key.data());
         }
#else
         try {
            // �w��̃L�[������
            if (_sounds.contains(key.data())) {
               auto&& [soundData, handle] = _sounds[key.data()];
               // �����n���h���̉��
               DeleteSoundMem(handle);
               // �w��̃L�[�̍폜
               _sounds.erase(key.data());
               // �G���[���b�Z�[�W���o�͂���
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�̉������ēx�A�ǂݍ��܂�܂����B�ȑO�̉����n���h���f�[�^�͔j������܂��B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �������̎擾
         auto [soundData, isLoad] = soundDataAndIsLoad;
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         auto handle = -1;
         // ���O�ǂݍ��݂���
         if (isLoad) {
            // �t�@�C�����̎擾
            auto fileName = soundData.fileName();
            // 3D�T�E���h�ɂ��邩
            SetCreate3DSoundFlag(is3Dsound);
            // ������ǂݍ��݁A�n���h���ɓo�^����
            handle = LoadSoundMem(fileName.data());
         }
         // �L�[�ƃf�[�^�������}�b�v�ɓo�^
         _sounds.emplace(key, std::make_pair(soundData, handle));
      }

      /// ���t�@�C�����̎擾
      std::pair<SoundData, int> ResourceServer::GetSoundInfo(std::string_view key) {
#ifndef _DEBUG
         if (!_sounds.contains(key.data())) {
            return std::make_pair(SoundData(), -1);   // �L�[�����o�^
         }
#else
         try {
            if (!_sounds.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�������R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _sounds[key.data()];
      }


      /*---------�G�t�F�N�g�֌W---------*/


      void ResourceServer::ClearEffects() {
         for (auto&& [key, effectInfo] : _effects) {
            auto [handle, speed] = effectInfo;
            DeleteEffekseerEffect(handle);
         }
         _effects.clear();
      }

      void ResourceServer::LoadEffect(std::string_view key, std::tuple<std::string, double,double> effectInfo) {
#ifndef _DEBUG
         // �w��̃L�[������΍폜����
         if (_effects.contains(key.data())) {
            auto [handle, speed] = _effects[key.data()];
            DeleteEffekseerEffect(handle);
            _effects.erase(key.data());
         }
#else
         // �w��̃L�[������΍폜����
         try{
            if (_effects.contains(key.data())) {
               auto [handle, speed] = _effects[key.data()];
               DeleteEffekseerEffect(handle);
               // �w�肵���L�[�̍폜
               _effects.erase(key.data());
               // �G���[���b�Z�[�W���o�͂���
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�̃G�t�F�N�g���ēx�A�ǂݍ��܂�܂����B�ȑO�̃G�t�F�N�g�n���h���f�[�^�͔j������܂��B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �G�t�F�N�g���̎擾
         auto [fileName,scale,speed] = effectInfo;
         // �G�t�F�N�g�̓ǂݍ���
         auto handle = LoadEffekseerEffect(fileName.c_str(), static_cast<float>(scale));
         // �G�t�F�N�g�n���h���ƍĐ����x�̓o�^���s��
         _effects.emplace(key, std::make_pair(handle, speed));
      }

      std::pair<int, double> ResourceServer::GetEffectInfo(std::string_view key) {
#ifndef _DEBUG
         // �w��̃L�[��������΁A��̃f�[�^��Ԃ�
         if (!_effects.contains(key.data())) {
            return std::make_pair(-1, 0);
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_effects.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���G�t�F�N�g�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �w��̃L�[�̏����擾����
         return _effects[key.data()];
      }
   }
}