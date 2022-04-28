
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  ゲーム本体
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Game.h"
#include "ModeTitle.h"
#include "ModeOption.h"
#include "ModeAmg.h"
#include "ModeTeam.h"
#include "ModeLoading.h"
#include "ModePoor.h"
#include "ModeClearResult.h"
#include "ModeGameOver.h"
#include "ModeMissionFailed.h"
#include "ModeMissionCompleted.h"
#include "ModeBoss.h"
#include "ModeTutorial.h"
#include "ModeTutorialSelect.h"

namespace {
   constexpr auto ScreenWidth = 1920;
   constexpr auto ScreenHeight = 1080;
   constexpr auto ScreenDepth = 32;
}

using namespace FragmentValkyria::Game;

bool Game::Initialize(HINSTANCE hInstance) {
   //デバッグモードならウインドウモードに設定及び、デバッグログの出力を行う
#ifndef _DEBUG
   SetOutApplicationLogValidFlag(false);
   ChangeWindowMode(false);
#else
   SetOutApplicationLogValidFlag(true);
   ChangeWindowMode(true);
#endif
   //画面サイズ設定
   SetGraphMode(ScreenWidth, ScreenHeight, ScreenDepth);

   // DirectX11を使用可(Effekseerを使用する為)
   SetUseDirect3DVersion(DX_DIRECT3D_11);

   //エラーが起きたら直ちに終了
   if (DxLib_Init() == -1) {
      return false;
   }
#ifndef _DEBUG
   SetBackgroundColor(0, 0, 0);
#else
   SetBackgroundColor(0, 0, 255);
#endif
   //描画先を裏画面にセット
   SetDrawScreen(DX_SCREEN_BACK);

   // Effekseerの初期化
   if (Effekseer_Init(8000) == -1) {
      DxLib_End();
      return -1;
   }

   // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
   SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
   // DXライブラリのデバイスロストした時のコールバックの設定
   Effekseer_SetGraphicsDeviceLostCallbackFunctions();

   // Ｚバッファを有効にする
   SetUseZBuffer3D(TRUE);
   // Ｚバッファへの書き込みを有効にする
   SetWriteZBuffer3D(TRUE);

   _inputManager = std::make_unique < AppFrame::Input::InputManager > ();

   _soundComponent = std::make_unique< AppFrame::Sound::SoundComponent>();

   _loadresJson = std::make_unique< AppFrame::Resource::LoadResourceJson>();

   _texComponent = std::make_unique< AppFrame::Texture::TextureComponent>();

   _objFactory = std::make_unique<Create::ObjectFactory>();

   _objServer = std::make_unique<Object::ObjectServer>();

   _sprServer = std::make_unique <Sprite::SpriteServer>();

   _efcServer = std::make_unique<Effect::EffectServer>();

   _loadStage = std::make_unique<Stage::LoadStageFromJson>();
   // 各ファイルへのカレントパスをサーバーに登録する
   CurrentPathRegist();
   // 各モード(シーン)をサーバーに登録する
   ModeRegist();

   return true;
}

void Game::Run() {
   //メインループ
   while (_gState != GameState::Quit) {
      Input();  // 入力
      Update(); // 更新
      Render(); // 描画
   }
}

void Game::ShutDown() {
   Effkseer_End();
   DxLib_End();
}

void Game::Input() {
   if (ProcessMessage() == -1) {
      _gState = GameState::Quit;  // -1 が返ってきたのでゲームを終了する
   }
   // 入力状態の更新
   _inputManager->Update();
#ifdef _DEBUG
   if (_inputManager->GetKeyboard().EscClick()) {
      _gState = GameState::Quit;  // ESC押されたのでゲームを終了する
   }
#endif
   // モードサーバーの入力処理を実行
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Input(*_inputManager);
}

void Game::Update() {
   //モードの更新処理
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Update();
}

void Game::Render() {
   //モードの描画処理
   ClearDrawScreen();
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Render();
   ScreenFlip();
}

void Game::CurrentPathRegist(){
   const AppFrame::Path::CurrentPathServer::CurrentPathMap pathToUsed{
      {"Model","resource/model"},
      {"Texture","resource/graphic"},
      {"Sound","resource/sound"},
      {"Effect","resource/effect"},
      {"Stage","resource/stage"},
      {"Movie","resource/movie"},
      {"TextureJson","resource/json/graphic" },
      {"ModelJson","resource/json/model" },
      {"SoundJson","resource/json/sound" },
      {"EffectJson","resource/json/effect"},
      {"StageJson","resource/json/stage"},
      {"SpawnJson","resource/json/spawntable"},
      {"ParamJson","resource/json/param"}
   };
   auto& pathServer = AppFrame::Path::CurrentPathServer::GetInstance();
   pathServer.RegistCurrentPath(pathToUsed);
}

void Game::ModeRegist() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Register("Amg", std::make_shared<Mode::ModeAmg>());
   modeServer.PushBack("Amg");
   modeServer.PushBack("FadeIn");
   modeServer.Register("Team", std::make_shared<Mode::ModeTeam>());
   modeServer.Register("Title", std::make_shared<Mode::ModeTitle>());
   modeServer.Register("Option", std::make_shared<Mode::ModeOption>());
   modeServer.Register("Loading", std::make_shared<Mode::ModeLoading>());
   modeServer.Register("TutorialSelect", std::make_shared<Mode::ModeTutorialSelect>());
   _loadresJson->LoadEffects("ingame");
   SetUseASyncLoadFlag(true);
   modeServer.Register("Tutorial", std::make_shared<Mode::ModeTutorial>());
   modeServer.Register("Poor", std::make_shared<Mode::ModePoor>());
   modeServer.Register("Boss", std::make_shared<Mode::ModeBoss>());
   modeServer.Register("MissionFailed", std::make_shared<Mode::ModeMissionFailed>());
   modeServer.Register("MissionCompleted", std::make_shared<Mode::ModeMissionCompleted>());
   modeServer.Register("GameOver", std::make_shared<Mode::ModeGameOver>());
   modeServer.Register("ClearResult", std::make_shared<Mode::ModeClearResult>());
}
