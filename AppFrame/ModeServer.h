#pragma once
/*****************************************************************//**
 * \file   ModeServer.h
 * \brief  モードの一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include <unordered_map>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief モード関係
    */
   //二重インクルード防止
   namespace Input {
      class InputManager;
   }
   namespace Mode {
      /** 二重インクルード防止 */
      class ModeBase;
      /**
       * \class モードの一括管理クラス
       * \brief モードを登録し、一括管理を行う
       */
      class ModeServer {
      public:
         /**
          * \brief コンストラクタ
          * \param key モードのインスタンスを関連付ける任意の文字列
          * \param mode モードのインスタンス
          */
         ModeServer(std::string_view key, std::shared_ptr<ModeBase> mode);
         /**
          * \brief モードの登録
          * \param key モードのインスタンスに関連付ける任意の文字列
          * \param mode モードのインスタンス
          */
         void Register(std::string_view key, std::shared_ptr<ModeBase> mode);

         /**
          * \brief モードリストの一番後ろにモード挿入
          * \param key 登録済みのモードに関連付けた文字列
          */
         void PushBack(std::string_view key);
         /**
          * \brief モードリストの一番後ろのモードを削除
          */
         void PopBack();
         /**
          * \brief モードの遷移
          * \param key 登録済みのモードに関連付けた文字列
          * \param fadeType フェード時間の種別
          */
         void GoToMode(std::string_view key, char fadeType= 'M' );
         /**
          * \brief モードリストの一番後ろの真下にモード挿入
          * \param key 登録済みのモードに関連付けた文字列
          */
         void InsertBelowBack(std::string_view key);
         /**
          * \brief モードのインスタンスを返す
          * \param key 登録済みのモードに関連付けた文字列
          * \return モードのインスタンス
          */
         std::shared_ptr<ModeBase> GetMode(std::string_view key);
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(Input::InputManager& input);
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Render() const;

      private:
         /**
          * \brief モードリストの一番後ろの真下にフェード(アウトorイン)モード挿入し、フェード時間を設定する
          * \param key 登録済みのモードに関連付けた文字列
          * \param fadeType フェード時間の種別
          */
         void FadeInsertBelowBack(std::string_view key, char fadeType);
         /**
          * \brief モードリストの一番後ろにモード挿入
          * \param key 登録済みのモードに関連付けた文字列
          * \param fadeType フェード時間の種別
          */
         void FadePushBack(std::string_view key, char fadeType);

         std::unordered_map<std::string, std::shared_ptr<ModeBase>> _modeRegistry;  //!< モードを登録する連想配列
         std::list<std::shared_ptr<ModeBase>> _modeList;                            //!< モードの処理を回す双方向配列
      };
   }
}