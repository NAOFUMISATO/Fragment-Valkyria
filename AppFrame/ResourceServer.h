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
#include "SoundData.h"
#include "Singleton.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {
      /**
       * \class リソースの一括管理クラス
       * \brief リソースを読み込み、一括管理を行う
       */
      class ResourceServer:public Temp::Singleton<ResourceServer> {
      private:
         /**
          * \brief コンストラクタ
          */
         ResourceServer();
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~ResourceServer() = default;
      public:
         friend class Temp::Singleton<ResourceServer>;
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
          * \brief 画像の読み込み
          * \param key キーとなる任意の文字列
          * \param texture DxLib::LoadDivGraph用クラス
          */
         virtual void LoadTexture(std::string_view key, Texture& texture);
         /**
          * \brief 画像ハンドルの取得
          * \param key 画像情報に関連付けた任意の文字列
          * \param no ハンドル番号
          * \return 画像ハンドルを返す、無い場合は-1を返す
          */
         virtual int GetTexture(const std::string_view key, int no = 0);
         /**
          * \brief アニメーション画像ハンドル群の取得
          * \param key 画像情報に関連付けた任意の文字列
          * \return 画像ハンドルの動的配列
          */
         virtual std::vector<int> GetTextures(std::string_view key);
         /**
          * \brief 画像情報の取得
          * \param key 画像情報に関連付けた任意の文字列
          * \return 画像情報
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
          * \brief モデルの読み込み
          * \param key キーとなる任意の文字列
          * \param filename モデルファイル名
          * \return モデルハンドル
          */
         virtual int LoadModel(std::string_view key, const std::string_view filename);
         /**
          * \brief モデルハンドル
          * \param key キーとなる任意の文字列
          * \param no ハンドル番号
          * \return モデルハンドルを返す、無い場合は-1を返す
          */
         virtual std::pair<int, int> GetModel(std::string_view key, int no = 0);
         /**
          * \brief 指定モデルの指定アニメーション番号の取得
          * \param key モデルのキーとなる任意の文字列
          * \param animName モデル情報のアニメーション名
          * \return 指定のアニメーションのアニメ番号
          */
         virtual int GetModelAnimIndex(std::string_view key, std::string_view animName);


         /*----------音源関係----------*/


         /**
          * \brief サウンドコンテナの解放
          */
         virtual void ClearSounds();
         /**
          * \brief 音ファイルの読み込み
          * \param key キーとなる任意の文字列
          * \param soundDataAndIsLoad 音源データと事前読み込み有無のペア
          */
         virtual void LoadSound(std::string_view key, std::pair<SoundData, bool> soundDataAndIsLoad);
         /**
          * \brief 音ファイル情報の取得
          * \param key キーとなる任意の文字列
          * \return 音源データとハンドルのペア
          */
         virtual std::pair<SoundData, int> GetSoundInfo(std::string_view key);


         /*----------エフェクト関係----------*/


         /**
          * \brief エフェクトコンテナの解放
          */
         virtual void ClearEffects();
         /**
          * \brief エフェクトを連想配列に登録する
          * \param key キーとなる任意の文字列
          * \param effectInfo ファイル名と初期拡大率と再生速度のTuple型
          */
         virtual void LoadEffect(std::string_view key, std::tuple<std::string, double, double> effectInfo);
         /**
          * \brief 連想配列に登録したエフェクトのハンドル
          * \param key ハンドルに関連付けた任意の文字列
          * \return エフェクトハンドルと再生速度のペア
          */
         virtual std::pair<int, double> GetEffectInfo(std::string_view key);

      private:
         std::unordered_map<std::string, 
            std::pair<Texture, std::vector<int>>> _textures;         //!< 画像データクラスと画像ハンドルの連想配列
         std::unordered_map<std::string,std::tuple<std::vector<int>,
            std::unordered_map<std::string, int>>> _models;          //!< モデルハンドルとアニメマップの連想配列
         std::unordered_map<std::string, 
            std::pair<SoundData,int>> _sounds;                       //!< 音源データとハンドルの連想配列
         std::unordered_map<std::string,
            std::pair<int, double>> _effects;                        //!< エフェクトハンドルと再生速度の連想配列
      };
   }
}