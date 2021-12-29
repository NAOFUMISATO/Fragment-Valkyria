/*****************************************************************//**
 * \file   ResourceServver.cpp
 * \brief  ���\�[�X�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ResourceServer.h"
#include <DxLib.h>
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
         if (!_textures.contains(key.data())) {
            return -1;   // �摜���R���e�i�Ɏw��̃L�[��������΁A-1��Ԃ�
         }
         auto&& [divGraph, handles] = _textures[key.data()];
         auto handle = handles.at(no);    // �摜���R���e�ino�Ԗڂ̗v�f�̎Q�Ƃ�Ԃ�
         return handle;                   // ��L�̃n���h����Ԃ�
      }

      Texture ResourceServer::GetTextureInfo(std::string_view& key) {
         if (!_textures.contains(key.data())) {
            return Texture();    // �摜���R���e�i�Ɏw��̃L�[��������΁A�摜���̋�̃N���X��Ԃ�
         }
         auto&& [divGraph, handles] = _textures[key.data()];
         return divGraph;        // �w��̃L�[�̉摜���̃N���X��Ԃ�
      }

      /*----------3D�֌W----------*/

      void ResourceServer::ClearModels() {
         for (auto&& [key, model] : _models) {
            auto&& [filename, handles] = model;
            for (auto handle : handles) {
               MV1DeleteModel(handle);  // ���f�����̃R���e�i��S�ĉ񂵁A���f���̍폜���s��
            }
            handles.clear();            // �摜�n���h���R���e�i�̉��
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
            auto& [filename, handles] = _models[key.data()];
            for (auto handle : handles) {
               MV1DeleteModel(handle);   // �o�^�ς݂̏ꍇ�̓��f�����폜
            }
            handles.clear();             // ���f���n���h���R���e�i�̉��
            _models.erase(key.data());   // �w�肵���L�[�̍폜
         }
         auto handle = MV1LoadModel(filename.data());   // // DxLib::MV1LoadModel���R�s�[����
         std::vector<int> handles{ handle };
         _models.emplace(key, std::make_pair(filename.data(), handles)); // ���f�����R���e�i�ɍ쐬�����n���h�����i�[����
         return handle;    // ���f���n���h����Ԃ�
      }

      std::pair<int, int> ResourceServer::GetModel(std::string_view key, int no) {
         if (!_models.contains(key.data())) {
            // �L�[�����o�^
            return std::make_pair(-1, no);
         }
         auto& [filename, handles] = _models[key.data()];
         if (no < handles.size()) {
            return std::make_pair(handles[no], no); // ����no�̏ꍇ
         }
         // �V�Kno�̏ꍇ�͕�������
         auto handle = MV1DuplicateModel(handles[0]);  /// handles[0]���I���W�i��
         handles.push_back(handle);
         return std::make_pair(handle, static_cast<int>(handles.size()) - 1);
      }

      void ResourceServer::LoadSound(std::string_view key, std::tuple<std::string, bool, int> filename_isLoad_volume) {
         if (_sounds.contains(key.data())) {
            return;   // �w��̃L�[���L��ΕԂ�
         }
         auto&& [filename, isLoad, volume] = filename_isLoad_volume;
         auto handle = -1;
         if (isLoad) {
            handle = LoadSoundMem(filename.c_str());
         }
         // �L�[�ƃf�[�^��map�ɓo�^
         _sounds.emplace(key, std::make_tuple(filename, handle, volume));
      }

      /// ���t�@�C�����̎擾
      std::tuple<std::string, int, int> ResourceServer::GetSoundInfo(std::string_view key) {
         if (!_sounds.contains(key.data())) {
            // �L�[�����o�^
            return std::make_tuple("", -1, 0);
         }
         return _sounds[key.data()];
      }
   }
}