#pragma once
/*****************************************************************//**
 * \file   StageData.h
 * \brief  ステージ情報クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <tuple>
#include <string>
#include "Vector4.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief ステージ関係
    */
   namespace Stage {
      /**
       * \class ステージ情報クラス
       * \brief 各ステージパーツ情報を登録する
       */
      class StageData {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param fileName ファイル名
          * \param position 位置
          * \param rotation 回転角
          * \param scale 拡大率
          */
         StageData(std::string_view fileName, Vector4 position, Vector4 rotation, Vector4 scale) :
            _fileName(fileName), _position(position), _rotation(rotation), _scale(scale) {
         };
         /**
          * \brief コンストラクタ
          */
         StageData() {};
         /**
          * \brief ファイル名の取得
          * \return ファイル名
          */
         std::string_view fileName() const { return _fileName; }
         /**
          * \brief ステージ情報の取得
          * \return 位置、回転角、拡大率のTuple型
          */
         std::tuple<Vector4, Vector4, Vector4> GetStageParams() const { return { _position,_rotation,_scale }; }

      private:
         std::string _fileName;    //!< ファイル名
         Vector4 _position;        //!< 位置
         Vector4 _rotation;        //!< 回転角
         Vector4 _scale;           //!< 拡大率
      };
   }
}
