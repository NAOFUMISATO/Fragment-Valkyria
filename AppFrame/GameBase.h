#pragma once
/*****************************************************************//**
 * \file   GameBase.h
 * \brief  ゲーム本体の基底クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <memory>
#include <tuple>
#include <DxLib.h>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
   namespace Mode {
      class ModeServer;
   }
   namespace Resource {
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Path {
      class CurrentPathServer;
   }
   namespace Sound {
      class SoundComponent;
   }
   namespace Effect {
      class EffectServer;
   }
   namespace Object {
      class ObjectServer;
   }
   namespace Texture {
      class SimpleTextureComponent;
   }
   namespace Sprite {
      class SpriteServer;
   }
   /**
    * \brief ゲームベース
    */
   namespace Game {
      /**
       * \class アプリケーションの基底クラス
       * \brief ゲームごとに継承して定義する
       */
      class GameBase {
      public:
         /**
          * \brief ゲームの状態列挙
          */
         enum class GameState {
            Play,    //!< 実行中
            Paused,  //!< 一時停止
            Quit     //!< 終了
         };
         /**
          * \brief コンストラクタ
          */
         GameBase();
         /**
          * \brief デストラクタ
          */
         virtual ~GameBase();
         /**
          * \brief 初期化処理
          * \param hInstance WinMainの第一引数
          * \return 初期化成功したか
          */
         virtual bool Initialize(HINSTANCE hInstance);
         /**
          * \brief メインループを実行する
          */
         virtual void Run();
         /**
          * \brief メインループを終了する
          */
         virtual void ShutDown();
         /**
          * \brief メインループ内で実行する入力処理
          */
         virtual void Input();
         /**
          * \brief メインループ内で実行する更新処理
          */
         virtual void Update();
         /**
          * \brief メインループ内で実行する描画処理
          */
         virtual void Render();

         /**
          * \brief シャドウマップハンドルの取得
          * \return シャドウマップハンドル
          */
         int shadowMapHandle() const { return _shadowMapHandle; }
         /**
          * \brief ゲームの基底クラスのインスタンスを取得
          * \return ゲームの基底クラスのインスタンス
          */
         inline static GameBase* gameInstance() { return _gameInstance; }
         /**
          * \brief モードサーバーの参照を取得
          * \return モードサーバーのポインタ
          */
         inline Mode::ModeServer& modeServer() const { return *_modeServer; }
         /**
          * \brief リソースサーバーの参照を取得
          * \return リソースサーバーのポインタ
          */
         inline Resource::ResourceServer& resServer() const { return *_resServer; }
         /**
          * \brief 入力の一括管理クラスの参照を取得
          * \return 入力の一括管理クラスのポインタ
          */
         inline Input::InputManager& inputManager() const { return *_inputManager; }
         /**
          * \brief パスサーバーの参照を取得
          * \return パスサーバーのポインタ
          */
         inline Path::CurrentPathServer& pathServer() const { return *_pathServer; }
         /**
          * \brief サウンド管理サーバーの参照を取得
          * \return サウンド管理サーバーのポインタ
          */
         inline Sound::SoundComponent& soundComponent() const { return *_soundComponent; }
         /**
          * \brief jsonファイル管理の参照を取得
          * \return jsonファイル管理のポインタ
          */
         inline Resource::LoadResourceJson& loadresJson() const { return *_loadresJson; }
         /**
          * \brief エフェクトサーバーの参照を取得
          * \return エフェクトサーバーのポインタ
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }
         /**
          * \brief オブジェクトサーバーの参照を取得
          * \return オブジェクトサーバーのポインタ
          */
         inline Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief 画像の簡易描画クラスの参照を取得
          * \return 画像の簡易描画クラスのポインタ
          */
         inline Texture::SimpleTextureComponent& simpleTexComponent() const { return *_simpleTexComponent; }
         inline Sprite::SpriteServer& sprServer() const { return *_sprServer; }
         /**
          * \brief ウィンドウモードに設定するか
          * \param _window_mode TRUEでウィンドウモードで実行、でなければフルスクリーンモードで実行
          */
         inline virtual void AppWindowMode(bool windowMode) { ChangeWindowMode(windowMode); }
         /**
          * \brief デバッグログを出力するか
          * \param _window_mode TRUEでデバッグログ出力
          */
         inline virtual void OutputDebugLog(bool windowMode) { SetOutApplicationLogValidFlag(windowMode); };
         /**
          * \brief 画面設定の値を返す
          * \return 画面横サイズ、画面縦サイズ、画面ビット数
          */
         inline virtual std::tuple<int, int, int> GraphSize() { return { 1280,1024,32 }; }

      protected:
         int _shadowMapHandle{ -1 };            //!< シャドウマップハンドル
         static GameBase* _gameInstance;        //!< ゲームのインスタンス
         GameState _gState{ GameState::Play };                       //!< ゲーム状態
         std::unique_ptr<Mode::ModeServer> _modeServer;              //!< モードの一括管理クラス
         std::unique_ptr<Resource::ResourceServer> _resServer;       //!< リソースの一括管理クラス
         std::unique_ptr<Input::InputManager> _inputManager;         //!< 入力の一括管理クラス
         std::unique_ptr<Path::CurrentPathServer> _pathServer;       //!< パスの一括管理クラス
         std::unique_ptr<Sound::SoundComponent> _soundComponent;     //!< サウンドの一括管理クラス
         std::unique_ptr<Resource::LoadResourceJson> _loadresJson;   //!< jsonファイル管理クラス
         std::unique_ptr<Effect::EffectServer> _efcServer;           //!< エフェクトの一括管理クラス
         std::unique_ptr<Object::ObjectServer> _objServer;           //!< オブジェクトの一括管理クラス
         std::unique_ptr<Texture::SimpleTextureComponent> _simpleTexComponent;   //!< 画像の簡易描画クラス
         std::unique_ptr<Sprite::SpriteServer> _sprServer;
      };
   }
}