#pragma once
/*****************************************************************//**
 * \file   LoadStageFromJson.h
 * \brief  ステージ情報の管理クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <filesystem>
#include <utility>
#include <unordered_map>
#include "StageData.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria{
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * ステージ関係
    */
   namespace Stage {
      /**
       * \class ステージ情報の管理クラス
       * \brief ステージ情報をJsonに読み込み、取得する
       */
      class LoadStageFromJson {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲームの参照
          */
         LoadStageFromJson(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         ~LoadStageFromJson()=default;
         /**
          * \brief ステージ情報の連想配列
          */
         void ClearStageModels();
         /**
          * \brief ステージ情報を連想配列に登録する
          * \param key 連想配列に登録するキー
          * \param stageData ステージ情報
          */
         void LoadStageModel(std::string_view key,StageData& stageData);
         /**
          * \brief ステージ情報をJsonに読み込む
          * \param jsonName Jsonファイル名
          */
         void LoadStageModels(const std::filesystem::path jsonName);
         /**
          * \brief ステージ情報の取得
          * \param key 各ステージに関連付けた任意の文字列
          * \return ステージパーツのハンドルとステージ情報のペア型の動的配列
          */
         std::vector<std::pair<int ,StageData>> GetStageModels(std::string_view key);

      private:
         Game::GameMain& _gameMain;                                   //!< ゲームの参照
         std::unordered_map<std::string, 
            std::vector<std::pair<int,StageData>>> _stageModelsMap;   //!< 各ステージ情報の連想配列
      };
   }
}
