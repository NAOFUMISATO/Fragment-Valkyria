
/*****************************************************************//**
 * \file   LargeEnemyCreator.cpp
 * \brief  ���[�W�G�l�~�[����
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemyCreator.h"
#include "Game.h"
#include "LargeEnemy.h"
#include "LargeEnemyHP.h"
#include "ModelAnimeComponent.h"
#include "ObjectBase.h"
#include "ObjectServer.h"
#include "SpriteServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> LargeEnemyCreator::Create() {
   using Vector4 = AppFrame::Math::Vector4;
   // ���[�W�G�l�~�[�̐���
   auto largeEnemy = std::make_unique<Enemy::LargeEnemy>();
   // ���[�W�G�l�~�[�̃A�j���[�V�����ꊇ�Ǘ��N���X�̐���
   auto model = std::make_unique<Model::ModelAnimeComponent>(*largeEnemy);
   // ���f���̐ݒ�
   model->SetModel("LargeEnemy");
   // ���[�W�G�l�~�[�̃A�j���[�V�����ꊇ�Ǘ��N���X�̐ݒ�
   largeEnemy->modelAnimeComponent(std::move(model));

   // ���[�W�G�l�~�[�̏�Ԉꊇ�Ǘ��N���X�̐���
   auto state = std::make_unique<AppFrame::State::StateServer>("Fall",
      std::make_shared<Enemy::LargeEnemy::StateFall>(*largeEnemy));
   // ���[�W�G�l�~�[�̏�Ԃ̒ǉ��o�^
   state->Register("Idle", std::make_shared<Enemy::LargeEnemy::StateIdle>(*largeEnemy));
   state->Register("FallObject", std::make_shared<Enemy::LargeEnemy::StateFallObject>(*largeEnemy));
   state->Register("Consecutive", std::make_shared<Enemy::LargeEnemy::StateConsecutiveFallObject>(*largeEnemy));
   state->Register("Gatling", std::make_shared<Enemy::LargeEnemy::StateGatling>(*largeEnemy));
   state->Register("Move", std::make_shared<Enemy::LargeEnemy::StateMove>(*largeEnemy));
   state->Register("Die", std::make_shared<Enemy::LargeEnemy::StateDie>(*largeEnemy));
   state->Register("Laser", std::make_shared<Enemy::LargeEnemy::StateLaser>(*largeEnemy));
   state->Register("FanGatling", std::make_shared<Enemy::LargeEnemy::StateFanGatling>(*largeEnemy));
   state->Register("Stun", std::make_shared<Enemy::LargeEnemy::StateStun>(*largeEnemy));
   // ���[�W�G�l�~�[�̏�Ԉꊇ�Ǘ��N���X�̐ݒ�
   largeEnemy->stateServer(std::move(state));

   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[�łȂ���Ώ������X�L�b�v���Ė߂�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // ���[�W�G�l�~�[�̃J�����Ǘ��N���X���v���C���[�̃J�����Ǘ��N���X�ɐݒ�
      largeEnemy->cameraComponent(object->cameraComponent());
   }
   auto& sprServer = Game::Game::GetSprServer();
   sprServer.Add(std::make_unique<Enemy::LargeEnemyHP>());

   // ���[�W�G�l�~�[�̃C���X�^���X��Ԃ�
   return std::move(largeEnemy);
}
