#pragma once
/*****************************************************************//**
 * \file   SoundData.h
 * \brief  音データクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include <string_view>
#include <vector>
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {
      /**
       * \class 音データクラス
       * \brief 音データを登録する
       */
      class SoundData {
      public:
         /**
          * \brief コンストラクタ
          * \param fileName ファイル名
          * \param initVolume 初期音量
          * \param is3Dsound 3Dサウンドにするか
          * \param radius 音が聞こえる距離
          */
         SoundData(std::string_view fileName, int initVolume, bool is3Dsound, float radius) :
            _fileName{ fileName }, _volume{ initVolume }, _is3Dsound{ is3Dsound }, _radius{radius}{
         };
         /**
          * \brief 空のコンストラクタ
          */
         SoundData() {};
         /**
          * \brief ファイル名の取得
          * \return ファイル名
          */
         inline std::string_view fileName() { return _fileName; }
         /**
          * \brief 音データの取得
          * \return 音量、3Dサウンドかどうか、音の聞こえる距離のTuple型
          */
         inline std::tuple<int, bool, float> GetSoundParams() { return { _volume ,_is3Dsound,_radius }; }

      private:
         std::string _fileName{ "" };  //!< ファイル名
         int _volume{ 0 };             //!< 音量
         bool _is3Dsound{ false };     //!< 3Dサウンドかどうか
         float _radius{ 0.0f };        //!< 音の聞こえる距離
      };
   }
}
