#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  各モードの基底クラス
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include "GameBase.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
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
         inline ModeServer& GetModeServer() const { return _gameBase.modeServer(); };
         /**
          * \brief リソース一括管理クラスの参照をゲームベース経由で取得
          * \return リソース一括管理クラスの参照
          */
         inline Resource::ResourceServer& GetResServer() const { return _gameBase.resServer(); }
         /**
          * \brief 音源一括管理クラスの参照をゲームベース経由で取得
          * \return
          */
         inline Sound::SoundServer& GetSoundServer() const { return _gameBase.soundServer(); }
         /**
          * \brief jsonファイル管理クラスの参照をゲームベース経由で取得
          * \return jsonファイル管理クラスの参照
          */
         inline Resource::LoadJson& GetLoadJson() const { return _gameBase.loadJson(); }
         inline Object::ObjectServer& GetObjServer() const { return _gameBase.objServer(); }
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