
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  �Q�[���{��
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
   //�f�o�b�O���[�h�Ȃ�E�C���h�E���[�h�ɐݒ�y�сA�f�o�b�O���O�̏o�͂��s��
#ifndef _DEBUG
   SetOutApplicationLogValidFlag(false);
   ChangeWindowMode(false);
#else
   SetOutApplicationLogValidFlag(true);
   ChangeWindowMode(true);
#endif
   //��ʃT�C�Y�ݒ�
   SetGraphMode(ScreenWidth, ScreenHeight, ScreenDepth);

   // DirectX11���g�p��(Effekseer���g�p�����)
   SetUseDirect3DVersion(DX_DIRECT3D_11);

   //�G���[���N�����璼���ɏI��
   if (DxLib_Init() == -1) {
      return false;
   }
#ifndef _DEBUG
   SetBackgroundColor(0, 0, 0);
#else
   SetBackgroundColor(0, 0, 255);
#endif
   //�`���𗠉�ʂɃZ�b�g
   SetDrawScreen(DX_SCREEN_BACK);

   // Effekseer�̏�����
   if (Effekseer_Init(8000) == -1) {
      DxLib_End();
      return -1;
   }

   // �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
   SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
   // DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N�̐ݒ�
   Effekseer_SetGraphicsDeviceLostCallbackFunctions();

   // �y�o�b�t�@��L���ɂ���
   SetUseZBuffer3D(TRUE);
   // �y�o�b�t�@�ւ̏������݂�L���ɂ���
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
   // �e�t�@�C���ւ̃J�����g�p�X���T�[�o�[�ɓo�^����
   CurrentPathRegist();
   // �e���[�h(�V�[��)���T�[�o�[�ɓo�^����
   ModeRegist();

   return true;
}

void Game::Run() {
   //���C�����[�v
   while (_gState != GameState::Quit) {
      Input();  // ����
      Update(); // �X�V
      Render(); // �`��
   }
}

void Game::ShutDown() {
   Effkseer_End();
   DxLib_End();
}

void Game::Input() {
   if (ProcessMessage() == -1) {
      _gState = GameState::Quit;  // -1 ���Ԃ��Ă����̂ŃQ�[�����I������
   }
   // ���͏�Ԃ̍X�V
   _inputManager->Update();
#ifdef _DEBUG
   if (_inputManager->GetKeyboard().EscClick()) {
      _gState = GameState::Quit;  // ESC�����ꂽ�̂ŃQ�[�����I������
   }
#endif
   // ���[�h�T�[�o�[�̓��͏��������s
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Input(*_inputManager);
}

void Game::Update() {
   //���[�h�̍X�V����
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   modeServer.Update();
}

void Game::Render() {
   //���[�h�̕`�揈��
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
