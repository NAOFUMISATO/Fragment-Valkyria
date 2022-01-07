
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
