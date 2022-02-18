#pragma once
#include "ObjectBase.h"

namespace FragmentValkyria {
   /**
    * \brief オブジェクト関係
    */
   namespace Object {
      /**
       * \class オブジェクトの一括管理クラス
       * \brief ObjectBaseで登録するオブジェクトを一括管理する
       */
      class ObjectServer {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief オブジェクトを動的配列に追加する
          * \param object 追加するオブジェクト
          */
         void Add(std::unique_ptr<ObjectBase> object);
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの入力処理を回す
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの更新処理を回す
          */
         void Update();
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの描画処理を回す
          */
         void Render();
         /**
          * \brief 動的配列の解放
          */
         void Clear();
         /**
          * \brief オブジェクトのVector4データをレジストリに登録する
          * \param key Vector4データに関連付ける任意の文字列
          * \param vec 登録するVector4データ
          */
         void RegistVector(std::string_view key, Vector4 vecData);
         /**
          * \brief オブジェクトのdouble型データをレジストリに登録する
          * \param key double型データに関連付ける任意の文字列
          * \param doubleData 登録するdouble型データ
          */
         void RegistDouble(std::string_view key, double doubleData);
         /**
          * \brief オブジェクトの処理を回す動的配列を取得する
          */
         std::vector<std::unique_ptr<ObjectBase>>& runObjects() { return _runObjects; };
         /**
          * \brief レジストリに登録したオブジェクトのVecto4データを取得する
          * \param key オブジェクトのVecto4データに関連付けた任意の文字列
          * \return 登録したVector4データ
          */
         Vector4 GetVecData(std::string_view key);
         /**
          * \brief レジストリに登録したオブジェクトのdouble型データを取得する
          * \param key double型データに関連付けた任意の文字列
          * \return 登録したdoubleデータ
          */
         double GetDoubleData(std::string_view key);

      private:
         bool _updating{ false };                                  //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<ObjectBase>> _runObjects;     //!< オブジェクトの処理を回す用の動的配列
         std::vector<std::unique_ptr<ObjectBase>> _pendingObjects; //!< 保留中のオブジェクトを登録しておく動的配列
         std::unordered_map<std::string, Vector4> _objVecData;     //!< オブジェクトのVector4データを登録しておく連想配列
         std::unordered_map<std::string, double> _objDoubleData;   //!< オブジェクトのdoubleデータを登録しておく連想配列
      };
   }
}
