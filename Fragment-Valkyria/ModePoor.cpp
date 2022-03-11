
#include "ModePoor.h"
#include <stdexcept>
#include "BulletCreator.h"
#include "FallObjectCreator.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LargeEnemyCreator.h"
#include "LargeEnemy.h"
#include "LaserCreator.h"
#include "PlayerCreator.h"
#include "Player.h"
#include "PoorEnemyGatlingCreator.h"
#include "PoorEnemyMeleeCreator.h"
#include "PoorEnemyAlmightyCreator.h"
#include "StageCreator.h"
#include "GatlingCreator.h"
#include "ObjectBase.h"
#include "LoadStageFromJson.h"
#include "ModelAnimeComponent.h"

namespace {
   auto poorParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemy",
      { "max_wave" });
   const int MaxWave = poorParamMap["max_wave"];

   auto playerParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("player", { "max_hp","max_bullet","max_portion" });
   const double MaxHp = playerParamMap["max_hp"];
   const int MaxBullet = playerParamMap["max_bullet"];
   const int MaxPortion= playerParamMap["max_portion"];
}

using namespace FragmentValkyria::Mode;

ModePoor::ModePoor(Game::GameMain& gameMain) : ModeInGameBase{ gameMain } {
}

void ModePoor::Init() {
   GetLoadJson().LoadSounds("ingame");
   _gameMain.loadStage().LoadStageModels("Stage");
}

void ModePoor::Enter() {
   using Vector4 = AppFrame::Math::Vector4;

   auto& objFactory = GetObjFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>(_gameMain));
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>(_gameMain));
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>(_gameMain));
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>(_gameMain));
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>(_gameMain));
   objFactory.Register("PoorEnemyMelee", std::make_unique<Create::PoorEnemyMeleeCreator>(_gameMain));
   objFactory.Register("PoorEnemyAlmighty", std::make_unique<Create::PoorEnemyAlmightyCreator>(_gameMain));

   std::vector<std::string> spawnTableNames;
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "poorwave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("poor", spawnTableNames );

   objFactory.SetSpawnTable("poorwave1");

   auto player = objFactory.Create("Player");
   // �A�N�^�[�T�[�o�[�ɓo�^���ʃA�N�Z�X�p
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   GetSoundComponent().Stop("TitleBgm");
   GetSoundComponent().PlayLoop("PoorBattle");
   _wave = 1;
   _gameMain.ingameTimer(0);
   _gameMain.playerStatus(MaxHp, MaxBullet, MaxPortion);
   ModeInGameBase::Enter();
}

void ModePoor::Input(AppFrame::Input::InputManager& input) {
   //-----------��--------------
   if (input.GetXJoypad().BackClick()) {
      _gameMain.isPoorClear(true);
      GetModeServer().GoToMode("Loading",'S');
   }
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("ClearResult", 'S');
   }
   //-----------��--------------
   ModeInGameBase::Input(input);
}

void ModePoor::Update() {
   ModeInGameBase::Update();
   WaveProcess();
}

void ModePoor::Render() {
   ModeInGameBase::Render();
}

void ModePoor::WaveProcess() {
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X���珈�����񂷗p�̓��I�z����擾����
   auto&& runObjects = GetObjServer().runObjects();
   // ���I�z��Ɉ�v����v�f�����邩������s��
   auto isActiveEnemy = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // ������Ԃ̎G���G�͂��邩
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::PoorEnemy) && obj->IsActive(); });
   // ������Ԃ̎G���G�����Ȃ���
   if (!isActiveEnemy) {
      // �ő�wave�ɒB�����Ȃ烂�[�h�J�ڂ��s��
      if (_wave >= MaxWave) {
         _gameMain.isPoorClear(true);
         GetModeServer().GoToMode("Loading", 'S');
      }
      // ����wave�̃X�|�[���e�[�u����ݒ肷��
      GetObjFactory().SetSpawnTable("poorwave" + std::to_string(_wave + 1));
      // wave��i�߂�
      _wave++;
   }
}
