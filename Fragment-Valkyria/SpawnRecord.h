#pragma once
/*****************************************************************//**
 * \file   SpawnRecord.h
 * \brief  生成時の情報クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include <string_view>
#include "Vector4.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief 生成関係
    */
   namespace Create {
      /**
       * \class 生成時の情報クラス
       * \brief 生成時の情報を登録する
       */
      class SpawnRecord {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          */
         SpawnRecord() {};
         /**
          * \brief コンストラクタ
          * \param prograss 進捗
          * \param type 生成するオブジェクトの種別
          * \param position 位置
          * \param rotation 回転角
          */
         SpawnRecord(unsigned int prograss, std::string type, Vector4 position, Vector4 rotation) :
            _progress{ prograss }, _type{ type }, _position{ position }, _rotation{ rotation }{
         }
         /**
          * \brief 生成時の情報を取得する
          * \return 生成時の情報
          */
         std::tuple<unsigned int, std::string, Vector4, Vector4> GetSpawnParam() const { 
            return { _progress ,_type,_position,_rotation }; 
         }

      private:
         unsigned int _progress{ 0 };  //!< 進捗
         std::string _type{ "" };      //!< 生成するオブジェクトの種別文字列
         Vector4 _position{ 0,0,0 };   //!< 位置
         Vector4 _rotation{ 0,0,0 };   //!< 回転角
      };
   }
}
