/*****************************************************************//**
 * \file   ResourceServver.cpp
 * \brief  リソースの一括管理
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
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {
      ResourceServer::ResourceServer() {
         Init();
      }

      void ResourceServer::Init() {
         Release();        // リソースの全解放
      }

      void ResourceServer::Release() {
         ClearTextures();  // 画像情報の解放
         ClearModels();    // モデル情報の解放
         ClearSounds();    // 音源情報の解放
         ClearEffects();   // エフェクト情報の解放
      }

      /*----------2D関係----------*/

      void ResourceServer::ClearTextures() {
         for (auto&& [key, divGraphAndHandles] : _textures) {
            auto&& [divGraph, handles] = divGraphAndHandles;
            // 画像情報のコンテナを全て回し、画像の削除を行う
            for (auto&& handle : handles) {
               DeleteGraph(handle);
            }
            // 画像ハンドルコンテナの解放
            handles.clear();
         }
         // 画像情報のコンテナの解放
         _textures.clear();
      }

      void ResourceServer::LoadTexture(std::string_view key, Texture& texture) {
#ifndef _DEBUG
         // 画像情報コンテナから指定のキーを検索する
         if (_textures.contains(key.data())) {
            auto&& [texture, handles] = _textures[key.data()];
            // 登録済みの場合は画像を削除
            for (auto handle : handles) {
               DeleteGraph(handle);
            }
            // 画像ハンドルコンテナの解放
            handles.clear();
            // 指定したキーの削除
            _textures.erase(key.data());
         }
#else
         // 画像情報コンテナから指定のキーを検索する
         try {
            if (_textures.contains(key.data())) {
               auto&& [texture, handles] = _textures[key.data()];
               // 登録済みの場合は画像を削除
               for (auto handle : handles) {
                  DeleteGraph(handle);
               }
               // 画像ハンドルコンテナの解放
               handles.clear();
               // 指定したキーの削除
               _textures.erase(key.data());
               // 画像ハンドルのアドレスが変わるため、エラーメッセージを出力する
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]の画像が再度、読み込まれました。以前の画像ハンドルデータは破棄されます。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 各画像情報の取得
         auto filename = texture.fileName().data();
         auto [allnum, xnum, ynum, xsize, ysize] = texture.GetDivParams();
         std::vector<int> handles(allnum);
         // 画像を分割読み込みする
         LoadDivGraph(filename, allnum, xnum, ynum, xsize, ysize, handles.data());
         // 画像情報コンテナに作成したハンドルを格納する
         _textures.emplace(key, std::make_pair(texture, handles));
      }

      int ResourceServer::GetTexture(std::string_view key, int no) {
#ifndef _DEBUG
         // 画像情報コンテナに指定のキーが無ければ、-1を返す
         if (!_textures.contains(key.data())) {
            return -1;
         }
#else
         // 画像情報コンテナに指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー["+ message +"]が画像情報コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto&& [divGraph, handles] = _textures[key.data()];
         // 画像情報コンテナno番目の要素の参照を返す
         auto handle = handles.at(no);
         // 上記のハンドルを返す
         return handle;
      }

      std::vector<int> ResourceServer::GetTextures(std::string_view key) {
#ifndef _DEBUG
         // 画像情報コンテナに指定のキーが無ければ、空の動的配列を返す
         if (!_textures.contains(key.data())) {
            return std::vector<int>(0);
         }
#else
         // 画像情報コンテナに指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]が画像情報コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 指定のキーの、ハンドル群を返す
         auto&& [divGraph, handles] = _textures[key.data()];
         return handles;
      }

      Texture ResourceServer::GetTextureInfo(std::string_view key) {
#ifndef _DEBUG
         // 画像情報コンテナに指定のキーが無ければ、画像情報の空のクラスを返す
         if (!_textures.contains(key.data())) {
            return Texture();
         }
#else
         // 画像情報コンテナに指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_textures.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー["+ message +"]が画像情報コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 指定のキーの画像情報のクラスを返す
         auto&& [divGraph, handles] = _textures[key.data()];
         return divGraph;
      }

      /*----------3D関係----------*/

      void ResourceServer::ClearModels() {
         for (auto&& [key, model] : _models) {
            auto&& [handles,animes] = model;
            // モデル情報のコンテナを全て回し、モデルの削除を行う
            for (auto handle : handles) {
               MV1DeleteModel(handle);
            }
            // 画像ハンドルコンテナの解放
            handles.clear();
            // アニメ情報コンテナの解放
            animes.clear();
         }
         // モデル情報のコンテナの解放
         _models.clear();
      }

      void ResourceServer::DeleteDuplicateModels() {
         for (auto&& [key, model] : _models) {
            // オリジナルのハンドルは配列の先頭のため、それ以外の複製(Duplicate)したモデルは削除する
            auto&& [handles,animes] = model;
            // 0番がオリジナル
            auto original = handles[0];
            erase_if(handles, [original](auto handle) {
               // オリジナル以外のモデルハンドルは削除する
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
         // 指定のキーがあれば、モデルを削除
         if (_models.contains(key.data())) {
            auto& [handles,animes] = _models[key.data()];
            for (auto handle : handles) {
               MV1DeleteModel(handle);
            }
            // モデルハンドルコンテナの解放
            handles.clear();
            // アニメ情報コンテナの解放
            animes.clear();
            // 指定したキーの削除
            _models.erase(key.data());
         }
#else
         // 指定のキーがあれば、モデルを削除
         try {
            if (_models.contains(key.data())) {
               auto& [handles, animes] = _models[key.data()];
               for (auto handle : handles) {
                  MV1DeleteModel(handle);
               }
               // モデルハンドルコンテナの解放
               handles.clear();
               // アニメ情報コンテナの解放
               animes.clear();
               // 指定したキーの削除
               _models.erase(key.data());
               // モデルコンテナに指定のキーが無ければ、エラーメッセージを出力する
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]のモデルが再度、読み込まれました。以前のモデルハンドルデータは破棄されます。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // モデルを読み込み、ハンドルに登録する
         auto handle = MV1LoadModel(filename.data());
         // モデルハンドルを格納する動的配列の先頭にハンドルを登録する
         std::vector<int> handles{ handle };
         // アニメーション総数を取得
         auto animNum = MV1GetAnimNum(handle);
         // 連想配列に取得したアニメ名をキーとしたアニメーションを登録する
         std::unordered_map<std::string, int> animes;
         for (int i = 0; i < animNum; ++i) {
            auto animName = MV1GetAnimName(handle, i);
            animes.emplace(animName, i);
         }
         // モデル情報コンテナに作成したハンドルと、アニメ情報コンテナを登録する
         _models.emplace(key, std::make_pair(handles,animes));
         // モデルハンドルを返す
         return handle;
      }

      std::pair<int, int> ResourceServer::GetModel(std::string_view key, int no) {
#ifndef _DEBUG
         // 指定のキーが無ければ、空のデータを返す
         if (!_models.contains(key.data())) {
            return std::make_pair(-1, no);
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_models.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー["+ message +"]がモデル情報コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // モデル情報の取得
         auto& [handles,animes] = _models[key.data()];
         // noが既存noなら新しく登録する
         if (no < handles.size()) {
            return std::make_pair(handles[no], no);
         }
         // 新規noの場合は複製する
         auto handle = MV1DuplicateModel(handles[0]);
         // 動的配列に複製したモデルを登録する
         handles.push_back(handle);
         // 指定noのハンドルを返す
         return std::make_pair(handle, static_cast<int>(handles.size()) - 1);
      }

      int ResourceServer::GetModelAnimIndex(std::string_view key, std::string_view animName) {
#ifndef _DEBUG
         if (!_models.contains(key.data())) {
            return -1;   // キーが未登録
         }
#else
         try {
            if (!_models.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------アニメーションを検索しようとしましたが、キー[" + message + "]がモデル情報コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif

         auto& [handles, animes] = _models[key.data()];

#ifndef _DEBUG
         if (!animes.contains(animName.data())) {
             return -1;   // キーが未登録
         }
#else
         try {
             if (!animes.contains(animName.data())) {
                 std::string message = animName.data();
                 throw std::logic_error("----------キー[" + message + "]がアニメ情報コンテナに存在しませんでした。----------\n");
             }
         }
         catch (std::logic_error& error) {
             OutputDebugString(error.what());
         }
#endif

         return animes[animName.data()];
      }

      /*-----------音源関係----------*/


      void ResourceServer::ClearSounds() {
         for (auto&& [key, sound] : _sounds) {
            auto&& [soudData, handle] = sound;
            DeleteSoundMem(handle);
         }
         _sounds.clear();
      }

      void ResourceServer::LoadSound(std::string_view key, std::pair<SoundData, bool> soundDataAndIsLoad) {
#ifndef _DEBUG
         // 指定のキーの検索
         if (_sounds.contains(key.data())) {
            auto&& [soudData, handle] = _sounds[key.data()];
            // 音源ハンドルの解放
            DeleteSoundMem(handle);
            // 指定のキーの削除
            _sounds.erase(key.data());
         }
#else
         try {
            // 指定のキーを検索
            if (_sounds.contains(key.data())) {
               auto&& [soundData, handle] = _sounds[key.data()];
               // 音源ハンドルの解放
               DeleteSoundMem(handle);
               // 指定のキーの削除
               _sounds.erase(key.data());
               // エラーメッセージを出力する
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]の音源が再度、読み込まれました。以前の音源ハンドルデータは破棄されます。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 音源情報の取得
         auto [soundData, isLoad] = soundDataAndIsLoad;
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         auto handle = -1;
         // 事前読み込みあり
         if (isLoad) {
            // ファイル名の取得
            auto fileName = soundData.fileName();
            // 3Dサウンドにするか
            SetCreate3DSoundFlag(is3Dsound);
            // 音源を読み込み、ハンドルに登録する
            handle = LoadSoundMem(fileName.data());
         }
         // キーとデータを音源マップに登録
         _sounds.emplace(key, std::make_pair(soundData, handle));
      }

      /// 音ファイル情報の取得
      std::pair<SoundData, int> ResourceServer::GetSoundInfo(std::string_view key) {
#ifndef _DEBUG
         if (!_sounds.contains(key.data())) {
            return std::make_pair(SoundData(), -1);   // キーが未登録
         }
#else
         try {
            if (!_sounds.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]が音源コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _sounds[key.data()];
      }


      /*---------エフェクト関係---------*/


      void ResourceServer::ClearEffects() {
         for (auto&& [key, effectInfo] : _effects) {
            auto [handle, speed] = effectInfo;
            DeleteEffekseerEffect(handle);
         }
         _effects.clear();
      }

      void ResourceServer::LoadEffect(std::string_view key, std::tuple<std::string, double,double> effectInfo) {
#ifndef _DEBUG
         // 指定のキーがあれば削除する
         if (_effects.contains(key.data())) {
            auto [handle, speed] = _effects[key.data()];
            DeleteEffekseerEffect(handle);
            _effects.erase(key.data());
         }
#else
         // 指定のキーがあれば削除する
         try{
            if (_effects.contains(key.data())) {
               auto [handle, speed] = _effects[key.data()];
               DeleteEffekseerEffect(handle);
               // 指定したキーの削除
               _effects.erase(key.data());
               // エラーメッセージを出力する
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]のエフェクトが再度、読み込まれました。以前のエフェクトハンドルデータは破棄されます。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // エフェクト情報の取得
         auto [fileName,scale,speed] = effectInfo;
         // エフェクトの読み込み
         auto handle = LoadEffekseerEffect(fileName.c_str(), static_cast<float>(scale));
         // エフェクトハンドルと再生速度の登録を行う
         _effects.emplace(key, std::make_pair(handle, speed));
      }

      std::pair<int, double> ResourceServer::GetEffectInfo(std::string_view key) {
#ifndef _DEBUG
         // 指定のキーが無ければ、空のデータを返す
         if (!_effects.contains(key.data())) {
            return std::make_pair(-1, 0);
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_effects.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がエフェクトコンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 指定のキーの情報を取得する
         return _effects[key.data()];
      }
   }
}