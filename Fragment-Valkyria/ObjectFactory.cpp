
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

namespace {
   constexpr auto OneFrame = 60;
}

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

void ObjectFactory::SetSpawnTable(std::string_view key) {
    _spawnProgress = 0;
    _progress = 0;
    auto spawnTable = GetSpawnTable(key);
    _spawnTable = spawnTable;
}

void ObjectFactory::UpdateSpawn() {
    while (_spawnTable.size() > _spawnProgress) {
        auto& spawnRecord = _spawnTable[_spawnProgress];
        auto [progress, key, position, rotation] = spawnRecord.GetSpawnParam();
        progress = progress * OneFrame;
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

void ObjectFactory::LoadSpawnTable(std::string_view key, SpawnTable& spawnTable) {
   if (_spawnTableMap.contains(key.data())) {
      _spawnTableMap[key.data()].clear();
      _spawnTableMap.erase(key.data());   // 指定したキーの削除
   }
   _spawnTableMap.emplace(key, spawnTable);
}

void ObjectFactory::LoadSpawnTables(const std::filesystem::path jsonName, const std::vector<std::string_view> tableNames) {
   namespace AppMath = AppFrame::Math;
   auto jsonDirectory = _gameMain.pathServer().GetCurrentPath("SpawnJson");
   auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
   std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
   try {
      if (reading.fail()) {
         throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
      }
   }
   catch (std::logic_error& e) {
      OutputDebugString(e.what());
   }
#endif
   nlohmann::json value;
   reading >> value;
   reading.close();
   auto spawnArray = value[jsonName.generic_string()];
   for (auto i = 0; i < tableNames.size(); i++) {
      SpawnTable spawnTable;
      for (auto& spawnType : spawnArray) {
         for (auto& spawnParam : spawnType[tableNames[i].data()]) {
            const auto progress = spawnParam["progress"];
            const auto type = spawnParam["type"];
            const auto position = spawnParam["position"];
            Vector4 spawnPosition = Vector4();
            for (auto& positionParam : position) {
               const auto x = positionParam["x"];
               const auto y = positionParam["y"];
               const auto z = positionParam["z"];
               spawnPosition.SetVec3(x, y, z);
            }
            const auto rotation = spawnParam["rotation"];
            Vector4 spawnRotation = Vector4();
            for (auto& rotationParam : rotation) {
               const auto x = rotationParam["x"];
               const auto y = rotationParam["y"];
               const auto z = rotationParam["z"];
               spawnRotation.SetVec3(x, y, z);
            }
            auto spawnRecord = SpawnRecord(progress, type, spawnPosition, spawnRotation);
            spawnTable.emplace_back(spawnRecord);
         }
      }
      LoadSpawnTable(tableNames[i], spawnTable);
   }
}

ObjectFactory::SpawnTable ObjectFactory::GetSpawnTable(std::string_view key) {
   if (!_spawnTableMap.contains(key.data())) {
      return SpawnTable{ SpawnRecord() };
   }
   return _spawnTableMap[key.data()];
}
