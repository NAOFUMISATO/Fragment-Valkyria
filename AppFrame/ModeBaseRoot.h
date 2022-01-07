#pragma once
/*****************************************************************//**
 * \file   ModeBaseRoot.h
 * \brief  各モードの基底クラス
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   // 二重インクルード防止
   namespace Game {
      class GameBase;
   }
   namespace Resource {
      class ResourceServer;
      class LoadJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Sound {
      class SoundServer;
   }
   namespace Object {
      class ObjectServer;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      //二重インクルード防止
      class ModeServer;
      /**
       * \class 各モードの基底クラス
       * \brief ModeServerに登録するモードの基底クラス
       *        このクラスを継承してモードを定義する
       */
      class ModeBaseRoot {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase ゲームベースの参照
          */
         ModeBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief デストラクタ
          */
         virtual ~ModeBaseRoot();
         /**
          * \brief 初期化処理
          */
         virtual void Init() = 0;
         /**
          * \brief 入口処理
          */
         virtual void Enter() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 更新処理
          */
         virtual void Render() {};
         /**
          * \brief 出口処理
          */
         virtual void Exit() {};

         /**
          * \brief ゲームベースの参照を取得
          * \return ゲームベースの参照
          */
         inline Game::GameBase& GetGameBase() const { return _gameBase; }
         /**
          * \brief モード一括管理クラスの参照をゲームベース経由で取得
          * \return モード管理クラスの参照
          */
         ModeServer& GetModeServer() const;
         /**
          * \brief リソース一括管理クラスの参照をゲームベース経由で取得
          * \return リソース一括管理クラスの参照
          */
         Resource::ResourceServer& GetResServer() const;
         /**
          * \brief 音源一括管理クラスの参照をゲームベース経由で取得
          * \return
          */
         Sound::SoundServer& GetSoundServer() const;
         /**
          * \brief jsonファイル管理クラスの参照をゲームベース経由で取得
          * \return jsonファイル管理クラスの参照
          */
         Resource::LoadJson& GetLoadJson() const;
         /**
          * \brief オブジェクト一括管理クラスの参照をゲームベース経由で取得
          * \return オブジェクト一括管理クラスの参照
          */
         Object::ObjectServer& GetObjServer() const;
         /**
          * \brief フェード時間の設定
          * \param fadeType フェード時間を指定する文字
          */
         inline void fadeType(char fadeType) { _fadeType = fadeType; };

      protected:
         Game::GameBase& _gameBase;   //!< ゲームベースの参照
         char _fadeType{ 'M' };       //!< フェード時間を指定する文字
      };
   }
}