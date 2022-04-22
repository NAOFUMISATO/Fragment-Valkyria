#pragma once
/*****************************************************************//**
 * \file   Singleton.h
 * \brief  シングルトンクラステンプレート
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <memory>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief テンプレート関係
    */
   namespace Temp {
      template<class T>
      /**
       * \class シングルトンクラステンプレート
       * \brief プロジェクトに1つのみ存在するクラスはこれを継承する
       */
      class Singleton {
      protected:
         /**
          * \brief コンストラクタ(外部でのインスタンス生成は禁止)
          */
         Singleton() {}
         /**
          * \brief デストラクタ
          */
         virtual ~Singleton() {}

      public:
         /**
          * \brief インスタンスの生成
          * \return 派生クラスのインスタンス
          */
         static inline T& GetInstance()
         {
            static T instance;
            return instance;
         }

      private:
         /**
          * \brief 代入演算子禁止
          */
         void operator=(const Singleton& obj) {}
         /**
          * \brief コピーコンストラクタ禁止
          */
         Singleton(const Singleton& obj) {}
      };
   }
}