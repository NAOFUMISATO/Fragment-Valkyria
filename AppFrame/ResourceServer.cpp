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
#include "PathServer.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      ResourceServer::ResourceServer(Game::GameBase& gameBase) : _gameBase(gameBase) {
         Init();
      }

      void ResourceServer::Init() {
         Release();   // �摜���y�у��f�����̏������i����j
      }

      void ResourceServer::Release() {
         ClearTextures();  // �摜���̉��
         ClearModels();    // ���f�����̉��
         ClearSounds();
         ClearEffects();
      }

      /*----------2D�֌W----------*/

      void ResourceServer::ClearTextures() {
         for (auto&& [key, divGraph_handles] : _textures) {
            auto&& [divGraph, handles] = divGraph_handles;
            for (auto&& handle : handles) {
               DeleteGraph(handle); // �摜���̃R���e�i��S�ĉ񂵁A�摜�̍폜���s��
            }
            handles.clear();        // �摜�n���h���R���e�i�̉��
         }
         _textures.clear();         // �摜���̃R���e�i�̉��
      }

      void ResourceServer::LoadTexture(std::string_view key, Texture& texture) {
         if (_textures.contains(key.data())) {
            auto&& [texture, handles] = _textures[key.data()];
            for (auto handle : handles) {
               DeleteGraph(handle); // �o�^�ς݂̏ꍇ�͉摜���폜
            }
            handles.clear();        // �摜�n���h���R���e�i�̉��
            _textures.erase(key.data());  // �w�肵���L�[�̍폜
         }
         auto filename = texture.GetTextureName().data();
         auto [allnum, xnum, ynum, xsize, ysize] = texture.GetDivParams();
         std::vector<int> handles(allnum);
         LoadDivGraph(filename, allnum, xnum, ynum, xsize, ysize, handles.data());   // DxLib::LoadDivGraph���R�s�[����
         _textures.emplace(key, std::make_pair(texture, handles));                 // �摜���R���e�i�ɍ쐬�����n���h�����i�[����
      }

      int ResourceServer::GetTexture(std::string_view key, int no) {
#ifndef _DEBUG
         if (!_textures.contains(key.data())) {
            return -1;   // �摜���R���e�i�Ɏw��̃L�[��������΁A-1��Ԃ�
         }
#else
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
         auto handle = handles.at(no);    // �摜���R���e�ino�Ԗڂ̗v�f�̎Q�Ƃ�Ԃ�
         return handle;                   // ��L�̃n���h����Ԃ�
      }

      Texture ResourceServer::GetTextureInfo(std::string_view key) {
#ifndef _DEBUG
         if (!_textures.contains(key.data())) {
            return Texture();    // �摜���R���e�i�Ɏw��̃L�[��������΁A�摜���̋�̃N���X��Ԃ�
         }
#else
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
         return divGraph;        // �w��̃L�[�̉摜���̃N���X��Ԃ�
      }

      /*----------3D�֌W----------*/

      void ResourceServer::ClearModels() {
         for (auto&& [key, model] : _models) {
            auto&& [handles,animes] = model;
            for (auto handle : handles) {
               MV1DeleteModel(handle);  // ���f�����̃R���e�i��S�ĉ񂵁A���f���̍폜���s��
            }
            handles.clear();            // �摜�n���h���R���e�i�̉��
            animes.clear();
         }
         _models.clear();               // ���f�����̃R���e�i�̉��
      }

      void ResourceServer::DeleteDuplicateModels() {
         for (auto&& [key, model] : _models) {
            auto&& [filename, handles] = model;
            auto original = handles[0];
            erase_if(handles, [original](auto handle) {
               if (original != handle) {
                  MV1DeleteModel(handle);   // �I���W�i���̃n���h���͔z��̐擪�̂��߁A����ȊO�̕���(Duplicate)�������f���͍폜����
                  return true;
               }
               return false;
               });
         }
      }

      int ResourceServer::LoadModel(std::string_view key, const std::string_view filename) {
         if (_models.contains(key.data())) {
            auto& [handles,animes] = _models[key.data()];
            for (auto handle : handles) {
               MV1DeleteModel(handle);   // �o�^�ς݂̏ꍇ�̓��f�����폜
            }
            handles.clear();             // ���f���n���h���R���e�i�̉��
            animes.clear();
            _models.erase(key.data());   // �w�肵���L�[�̍폜
         }
         auto handle = MV1LoadModel(filename.data());   // // DxLib::MV1LoadModel���R�s�[����
         std::vector<int> handles{ handle };

         auto animNum = MV1GetAnimNum(handle);
         std::unordered_map<std::string, int> animes;
         for (int i = 0; i < animNum; ++i) {
            auto animName = MV1GetAnimName(handle, i);
            animes.emplace(animName, i);
         }

         _models.emplace(key, std::make_pair(handles,animes)); // ���f�����R���e�i�ɍ쐬�����n���h�����i�[����
         return handle;    // ���f���n���h����Ԃ�
      }

      std::pair<int, int> ResourceServer::GetModel(std::string_view key, int no) {
#ifndef _DEBUG
         if (!_models.contains(key.data())) {
            return std::make_pair(-1, no);   // �L�[�����o�^
         }
#else
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
         auto& [handles,animes] = _models[key.data()];
         if (no < handles.size()) {
            return std::make_pair(handles[no], no); // ����no�̏ꍇ
         }
         // �V�Kno�̏ꍇ�͕�������
         auto handle = MV1DuplicateModel(handles[0]);  /// handles[0]���I���W�i��
         handles.push_back(handle);
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
         if (!animes.contains(key.data())) {
            return -1;   // �L�[�����o�^
         }
#else
         try {
            if (!animes.contains(key.data())) {
               std::string message = key.data();
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
            auto&& [filename, handle, volume] = sound;
            DeleteSoundMem(handle);
         }
         _sounds.clear();
      }

      void ResourceServer::LoadSound(std::string_view key, std::tuple<std::string, bool, int> soundInfo) {
         if (_sounds.contains(key.data())) {
            auto&& [filename, handle, volume] = _sounds[key.data()];
            DeleteSoundMem(handle);
            _sounds.erase(key.data());
         }
         auto [filename, isLoad, volume] = soundInfo;
         auto handle = -1;
         if (isLoad) {
            handle = LoadSoundMem(filename.c_str());
         }
         // �L�[�ƃf�[�^��map�ɓo�^
         _sounds.emplace(key, std::make_tuple(filename, handle, volume));
      }

      /// ���t�@�C�����̎擾
      std::tuple<std::string, int, int> ResourceServer::GetSoundInfo(std::string_view key) {
#ifndef _DEBUG
         if (!_sounds.contains(key.data())) {
            return std::make_tuple("", -1, 0);   // �L�[�����o�^
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
         for (auto&& [key, handle] : _effects) {
            DeleteEffekseerEffect(handle);
         }
         _effects.clear();
      }

      void ResourceServer::LoadEffect(std::string_view key, std::pair<std::string, double> effectInfo) {
         if (_effects.contains(key.data())) {
            DeleteEffekseerEffect(_effects[key.data()]);
            _effects.erase(key.data());  // �w�肵���L�[�̍폜
         }
         auto [fileName,scale] = effectInfo;
         auto handle = LoadEffekseerEffect(fileName.c_str(), static_cast<float>(scale));
         _effects.emplace(key, handle);
      }

      int ResourceServer::GetEffectHandle(std::string_view key) {
#ifndef _DEBUG
         if (!_effects.contains(key.data())) {
            return -1;   // �L�[�����o�^
         }
#else
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
         return _effects[key.data()];
      }
   }
}