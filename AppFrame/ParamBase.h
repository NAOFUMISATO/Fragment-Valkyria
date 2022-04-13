#pragma once
/*****************************************************************//**
 * \file   ParamBase.h
 * \brief  各値の管理を行う基底クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include <unordered_map>
#include <utility>
#include <vector>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Vector4.h"
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   // 二重インクルード防止
   namespace Game {
      class GameBase;
   }
   namespace Math {
      class Vector4;
   }
   /**
    * \brief 値管理関係
    */
   namespace Param {
      /**
       * \class 各値の管理を行う基底クラス
       * \brief 各値をjsonファイルから読み込み、取得する
       */
      class ParamBase {
         public:
         /**
          * \brief コンストラクタ
          * \param gameBase ゲームベースの参照
          */
         ParamBase(Game::GameBase& gameBase,std::string_view key);
         /**
          * \brief int型の値を連想配列から取得する
          * \param paramName 値名
          * \return int型の値
          */
         int GetIntParam(std::string_view paramName);
         /**
          * \brief float型の値を連想配列から取得する
          * \param paramName 値名
          * \return float型の値
          */
         float GetFloatParam(std::string_view paramName);
         /**
          * \brief double型の値を連想配列から取得する
          * \param paramName 値名
          * \return double型の値
          */
         double GetDoubleParam(std::string_view paramName);
         /**
          * \brief bool型の値を連想配列から取得する
          * \param paramName 値名
          * \return bool型の値
          */
         bool GetBoolParam(std::string_view paramName);
         /**
          * \brief Vector4型の値を連想配列から取得する
          * \param paramName 値名
          * \return Vector4型の値
          */
         Math::Vector4 GetVecParam(std::string_view paramName);

      protected:
         /**
          * \brief int型の値をjsonファイルから読み込み、連想配列に登録する
          * \param paramNames 値名
          */
         void LoadIntParam(std::vector<std::string_view> paramNames);
         /**
          * \brief float型の値をjsonファイルから読み込み、連想配列に登録する
          * \param paramNames 値名
          */
         void LoadFloatParam(std::vector<std::string_view> paramNames);
         /**
          * \brief double型の値をjsonファイルから読み込み、連想配列に登録する
          * \param paramNames 値名
          */
         void LoadDoubleParam(std::vector<std::string_view> paramNames);
         /**
          * \brief boolの値をjsonファイルから読み込み、連想配列に登録する
          * \param paramNames 値名
          */
         void LoadBoolParam(std::vector<std::string_view> paramNames);
         /**
          * \brief Vector4型の値をjsonファイルから読み込み、連想配列に登録する
          * \param paramNames 値名
          */
         void LoadVecParam(std::vector<std::string_view> paramNames);
         /**
          * \brief jsonファイルを開き、コピーしたオブジェクトを返す
          * \return nlohmann::json型のオブジェクト
          */
         nlohmann::json JsonSetUp();

         Game::GameBase& _gameBase;                               //!< ゲームベースの参照
         std::string _key;                                        //!< jsonファイル名
         std::unordered_map<std::string, int> _intMap;            //!< int型の値を登録する連想配列
         std::unordered_map<std::string, float> _floatMap;        //!< float型の値を登録する連想配列
         std::unordered_map<std::string, double> _doubleMap;      //!< double型の値を登録する連想配列
         std::unordered_map<std::string, bool> _boolMap;          //!< bool型の値を登録する連想配列
         std::unordered_map<std::string, Math::Vector4> _vecMap;  //!< Vector4型の値を登録する連想配列
      };
   }
}