#pragma once
/*****************************************************************//**
 * \file   ResourceServer.h
 * \brief  リソースの一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <utility>
#include <tuple>
#include "Texture.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
   namespace Game {
      class GameBase;
   }
   /**
    * @brief リソース関係
    */
   namespace Resource {
      /**
       * \class リソースの一括管理クラス
       * \brief リソースを読み込み、一括管理を行う
       */
      class ResourceServer {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションの基底クラスの参照
          */
         ResourceServer(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~ResourceServer() = default;
         /**
          * \brief リソース初期化
          */
         virtual void Init();
         /**
          * \brief リソース解放
          */
         virtual void Release();

         /*----------2D関係----------*/

         /**
          * \brief 画像コンテナの解放
          */
         virtual void ClearTextures();
         /**
          * \brief         画像の読み込み
          * \param key     キーとなる任意の文字列
          * \param texture DxLib::LoadDivGraph用クラス
          */
         virtual void LoadTexture(std::string_view key, Texture& texture);
         /**
          * \brief     画像ハンドルの取得
          * \param key キー
          * \param no  ハンドル番号
          * \return    画像ハンドルを返す、無い場合は-1を返す
          */
         virtual int GetTexture(const std::string_view key, int no = 0);
         /**
          * \brief     画像情報の取得
          * \param key キー
          * \return    画像情報
          */
         virtual Texture GetTextureInfo(std::string_view key);


         /*----------3D関係----------*/

         /**
          * \brief モデルコンテナの解放
          */
         virtual void ClearModels();
         /**
          * \brief 複製したモデルだけ削除してクリアする
          */
         virtual void DeleteDuplicateModels();
         /**
          * \brief          モデルの読み込み
          * \param key      キーとなる任意の文字列
          * \param filename モデルファイル名
          * \return         モデルハンドル
          */
         virtual int LoadModel(std::string_view key, const std::string_view filename);
         /**
          * \brief     モデルハンドル
          * \param key キー
          * \param no  ハンドル番号
          * \return    モデルハンドルを返す、無い場合は-1を返す
          */
         virtual std::pair<int, int> GetModel(std::string_view key, int no = 0);


         /*----------音源関係----------*/


         /**
          * \brief サウンドコンテナの解放
          */
         virtual void ClearSounds();
         /**
          * \brief                 音ファイルの読み込み
          * \param key             キーとなる任意の文字列
          * \param filename_isLoad ファイル名と事前読み込み有無のペア
          */
         virtual void LoadSound(std::string_view key, std::tuple<std::string, bool, int> soundInfo);
         /**
          * \brief     音ファイル情報の取得
          * \param key キー
          * \return    音ファイル名, ハンドル
          */
         virtual std::tuple<std::string, int, int> GetSoundInfo(std::string_view key);


         /*----------エフェクト関係----------*/


         virtual void ClearEffects();
         virtual void LoadEffect(std::string_view key, std::pair<std::string, double> effectInfo);
         virtual std::pair<std::string, int> GetEffectInfo(std::string_view key);
      private:
         Game::GameBase& _gameBase;          //!< ゲームベースの参照
         std::unordered_map<std::string, std::pair<Texture, std::vector<int>>> _textures;    //!< 任意の文字列をキーにしてDivGraphと画像ハンドルのペアを管理
         std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> _models;  //!< 任意の文字列をキーにしてモデルファイル名とハンドルのペアを管理
         std::unordered_map<std::string, std::tuple<std::string, int, int>> _sounds;         //!< 任意の文字列をキーにして音ファイル名とハンドルと初期音量を管理
         std::unordered_map<std::string, std::pair<std::string, int>> _effects;              //!< 任意の文字列をキーにしてエフェクトファイル名とハンドルと初期拡大率を管理
      };
   }
}