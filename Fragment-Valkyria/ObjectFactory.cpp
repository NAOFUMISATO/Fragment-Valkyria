
/*****************************************************************//**
 * \file   ObjectFactory.cpp
 * \brief  オブジェクト生成の一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectFactory.h"
#include "ObjectBase.h"
#include "GameMain.h"
#include "ObjectServer.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

ObjectFactory::ObjectFactory(Game::GameMain& gameMain) : _gameMain{ gameMain } {
   Clear();
}

bool ObjectFactory::Register(std::string_view type, std::unique_ptr<CreatorBase> creator) {
   if (_creatorMap.contains(type.data())) {
      return false;
   }
   _creatorMap.emplace(type.data(), std::move(creator));
   return true;
}

std::unique_ptr<Object::ObjectBase> ObjectFactory::Create(std::string_view type) {
   if (!_creatorMap.contains(type.data())) {
      return nullptr;
   }
   auto&& creator = _creatorMap[type.data()];
   return creator->Create();
}

void ObjectFactory::Clear() {
   _creatorMap.clear();
}

void ObjectFactory::SetSpawnTable(SpawnTable spawnTable) {
    _spawnProgress = 0;
    _progress = 0;
    _spawnTable = spawnTable;
}

void ObjectFactory::UpdateSpawn() {
    while (_spawnTable.size() > _spawnProgress) {
        auto& spawnRecord = _spawnTable[_spawnProgress];
        auto& [progress, key, position, rotation] = spawnRecord;
        if (progress > _progress) {
            break;
        }
        else {
            auto&& object = Create(key);
            object->position(position);
            object->rotation(rotation);
            _gameMain.objServer().Add(std::move(object));
            ++_spawnProgress;
        }
    }
    ++_progress;
}
