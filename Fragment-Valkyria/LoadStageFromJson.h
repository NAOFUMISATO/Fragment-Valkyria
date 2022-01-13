#pragma once
#include <filesystem>
#include <utility>
#include <unordered_map>
namespace FragmentValkyria{
   namespace Game {
      class GameMain;
   }
   namespace Stage {
      class StageData;
      class LoadStageFromJson {
      public:
         LoadStageFromJson(Game::GameMain& gameMain);
         ~LoadStageFromJson()=default;
         void ClearStageModels();
         void LoadStageModel(std::string_view key,StageData& stageData);
         void LoadStageModels(const std::filesystem::path jsonName);
         std::vector<std::pair<int ,StageData>> GetStageModels(std::string_view key);
      private:
         Game::GameMain& _gameMain;
         std::unordered_map<std::string, std::vector<std::pair<int,StageData>>> _stageModelsMap;
      };

}
}
