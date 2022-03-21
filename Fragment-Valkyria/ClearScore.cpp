
/*****************************************************************//**
 * \file   ClearScore.cpp
 * \brief  クリアスコアを表示する
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ClearScore.h"
#include "GameMain.h"

namespace {
   // Jsonファイルから各値を取得する
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("clear", {
      "s_scoresec","a_scoresec","b_scoresec","c_scoresec" });
   const int SScoreSec = paramMap["s_scoresec"];
   const int AScoreSec = paramMap["a_scoresec"];
   const int BScoreSec = paramMap["b_scoresec"];
   const int CScoreSec = paramMap["c_scoresec"];
   // Jsonファイルから各Vector4データを取得する
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("clear", { "score_pos" });
   const auto ScorePos = vecParamMap["score_pos"];

   constexpr auto OneSecFrame = 60;
}

using namespace FragmentValkyria::Clear;

ClearScore::ClearScore(Game::GameMain& gameMain) :SpriteBase{ gameMain } {
}

void ClearScore::Init() {
   _position = ScorePos;
   auto handles = GetResServer().GetTextures("ClearScore");
   auto timer = static_cast<int>(_gameMain.ingameTimer());
   if (timer >= OneSecFrame * CScoreSec) {
      _grHandle = handles[3];
   }
   else if (timer >= OneSecFrame * BScoreSec && OneSecFrame * CScoreSec > timer) {
      _grHandle = handles[2];
   }
   else if (timer >= OneSecFrame * AScoreSec && OneSecFrame * BScoreSec > timer) {
      _grHandle = handles[1];
   }
   else if (timer >= OneSecFrame * SScoreSec && OneSecFrame * AScoreSec > timer) {
      _grHandle = handles[0];
   }
   else if (OneSecFrame * SScoreSec > timer) {
      _grHandle = handles[4];
   }
}

void ClearScore::Draw() {
   SpriteBase::Draw();
}
