/*****************************************************************//**
 * \file   ResourceServver.cpp
 * \brief  リソースの一括管理
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ResourceServer.h"
#include <DxLib.h>
#include "GameBase.h"
#include "PathServer.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {
      ResourceServer::ResourceServer(Game::GameBase& gameBase) : _gameBase(gameBase) {
         Init();
      }

      void ResourceServer::Init() {
         Release();   // 画像情報及びモデル情報の初期化（解放）
      }

      void ResourceServer::Release() {
         ClearTextures();  // 画像情報の解放
         ClearModels();    // モデル情報の解放
      }

      /*----------2D関係----------*/

      void ResourceServer::ClearTextures() {
         for (auto&& [key, divGraph_handles] : _textures) {
            auto&& [divGraph, handles] = divGraph_handles;
            for (auto&& handle : handles) {
               DeleteGraph(handle); // 画像情報のコンテナを全て回し、画像の削除を行う
            }
            handles.clear();        // 画像ハンドルコンテナの解放
         }
         _textures.clear();         // 画像情報のコンテナの解放
      }

      void ResourceServer::LoadTexture(std::string_view key, Texture& texture) {
         if (_textures.contains(key.data())) {
            auto&& [texture, handles] = _textures[key.data()];
            for (auto handle : handles) {
               DeleteGraph(handle); // 登録済みの場合は画像を削除
            }
            handles.clear();        // 画像ハンドルコンテナの解放
            _textures.erase(key.data());  // 指定したキーの削除
         }
         auto filename = texture.GetTextureName().data();
         auto [allnum, xnum, ynum, xsize, ysize] = texture.GetDivParams();
         std::vector<int> handles(allnum);
         LoadDivGraph(filename, allnum, xnum, ynum, xsize, ysize, handles.data());   // DxLib::LoadDivGraphをコピーする
         _textures.emplace(key, std::make_pair(texture, handles));                 // 画像情報コンテナに作成したハンドルを格納する
      }

      int ResourceServer::GetTexture(std::string_view key, int no) {
         if (!_textures.contains(key.data())) {
            return -1;   // 画像情報コンテナに指定のキーが無ければ、-1を返す
         }
         auto&& [divGraph, handles] = _textures[key.data()];
         auto handle = handles.at(no);    // 画像情報コンテナno番目の要素の参照を返す
         return handle;                   // 上記のハンドルを返す
      }

      Texture ResourceServer::GetTextureInfo(std::string_view& key) {
         if (!_textures.contains(key.data())) {
            return Texture();    // 画像情報コンテナに指定のキーが無ければ、画像情報の空のクラスを返す
         }
         auto&& [divGraph, handles] = _textures[key.data()];
         return divGraph;        // 指定のキーの画像情報のクラスを返す
      }

      /*----------3D関係----------*/

      void ResourceServer::ClearModels() {
         for (auto&& [key, model] : _models) {
            auto&& [filename, handles] = model;
            for (auto handle : handles) {
               MV1DeleteModel(handle);  // モデル情報のコンテナを全て回し、モデルの削除を行う
            }
            handles.clear();            // 画像ハンドルコンテナの解放
         }
         _models.clear();               // モデル情報のコンテナの解放
      }

      void ResourceServer::DeleteDuplicateModels() {
         for (auto&& [key, model] : _models) {
            auto&& [filename, handles] = model;
            auto original = handles[0];
            erase_if(handles, [original](auto handle) {
               if (original != handle) {
                  MV1DeleteModel(handle);   // オリジナルのハンドルは配列の先頭のため、それ以外の複製(Duplicate)したモデルは削除する
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
               MV1DeleteModel(handle);   // 登録済みの場合はモデルを削除
            }
            handles.clear();             // モデルハンドルコンテナの解放
            _models.erase(key.data());   // 指定したキーの削除
         }
         auto handle = MV1LoadModel(filename.data());   // // DxLib::MV1LoadModelをコピーする
         std::vector<int> handles{ handle };
         _models.emplace(key, std::make_pair(filename.data(), handles)); // モデル情報コンテナに作成したハンドルを格納する
         return handle;    // モデルハンドルを返す
      }

      std::pair<int, int> ResourceServer::GetModel(std::string_view key, int no) {
         if (!_models.contains(key.data())) {
            // キーが未登録
            return std::make_pair(-1, no);
         }
         auto& [filename, handles] = _models[key.data()];
         if (no < handles.size()) {
            return std::make_pair(handles[no], no); // 既存noの場合
         }
         // 新規noの場合は複製する
         auto handle = MV1DuplicateModel(handles[0]);  /// handles[0]がオリジナル
         handles.push_back(handle);
         return std::make_pair(handle, static_cast<int>(handles.size()) - 1);
      }

      void ResourceServer::LoadSound(std::string_view key, std::tuple<std::string, bool, int> filename_isLoad_volume) {
         if (_sounds.contains(key.data())) {
            return;   // 指定のキーが有れば返す
         }
         auto&& [filename, isLoad, volume] = filename_isLoad_volume;
         auto handle = -1;
         if (isLoad) {
            handle = LoadSoundMem(filename.c_str());
         }
         // キーとデータをmapに登録
         _sounds.emplace(key, std::make_tuple(filename, handle, volume));
      }

      /// 音ファイル情報の取得
      std::tuple<std::string, int, int> ResourceServer::GetSoundInfo(std::string_view key) {
         if (!_sounds.contains(key.data())) {
            // キーが未登録
            return std::make_tuple("", -1, 0);
         }
         return _sounds[key.data()];
      }
   }
}