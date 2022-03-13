
/*****************************************************************//**
 * \file   LightAndShadow.cpp
 * \brief  �����y�ї����e�̏������s��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "LightAndShadow.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   // json�t�@�C������l���擾����
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("lightshadow", { "fog_enuble","fog_red","fog_green","fog_blue",
      "fog_start","global_ambred","global_ambgreen","global_ambblue","global_ambalpha","alllight_enuble","alllight_difred",
      "alllight_difgreen","alllight_difblue","alllight_difalpha","fog_end" ,"fixedlight_area" ,"fixedlight_atten_first" ,
      "fixedlight_atten_second","fixedlight_atten_third","alllight_spcred","alllight_spcgreen","alllight_spcblue","alllight_spcalpha",
      "alllight_ambred","alllight_ambgreen","alllight_ambblue","alllight_ambalpha","fixedlight_difred","fixedlight_difgreen",
      "fixedlight_difblue","fixedlight_difalpha","fixedlight_spcred","fixedlight_spcgreen","fixedlight_spcblue","fixedlight_spcalpha",
      "fixedlight_ambred","fixedlight_ambgreen","fixedlight_ambblue","fixedlight_ambalpha","shadow_resolution" });
   const bool FogEnuble = paramMap["fog_enuble"];                             //!< �t�H�O�̗L��
   const int FogRed= paramMap["fog_red"];                                     //!< �t�H�O�F(��)
   const int FogGreen = paramMap["fog_green"];                                //!< �t�H�O�F(��)
   const int FogBlue = paramMap["fog_blue"];                                  //!< �t�H�O�F(��)
   const float FogStart = paramMap["fog_start"];                              //!< �t�H�O���n�܂鋗��
   const float FogEnd = paramMap["fog_end"];                                  //!< �t�H�O�̐F�݂̂ɂȂ鋗��
   const float GlobalAmbRed = paramMap["global_ambred"];                      //!< �S�Ẵ��f���ɓK����������(��)
   const float GlobalAmbGreen = paramMap["global_ambgreen"];                  //!< �S�Ẵ��f���ɓK����������(��)
   const float GlobalAmbBlue = paramMap["global_ambblue"];                    //!< �S�Ẵ��f���ɓK����������(��)
   const float GlobalAmbAlpha = paramMap["global_ambalpha"];                  //!< �S�Ẵ��f���ɓK����������(�����x)
   const bool AllLightEnuble = paramMap["alllight_enuble"];                   //!< �W�������̗L��
   const float AllLightDifRed = paramMap["alllight_difred"];                  //!< �W�������̊g�U���ˌ�(��)
   const float AllLightDifGreen = paramMap["alllight_difgreen"];              //!< �W�������̊g�U���ˌ�(��)
   const float AllLightDifBlue = paramMap["alllight_difblue"];                //!< �W�������̊g�U���ˌ�(��)
   const float AllLightDifAlpha = paramMap["alllight_difalpha"];              //!< �W�������̊g�U���ˌ�(�����x)
   const float AllLightSpcRed = paramMap["alllight_spcred"];                  //!< �W�������̋��ʔ��ˌ�(��)
   const float AllLightSpcGreen = paramMap["alllight_spcgreen"];              //!< �W�������̋��ʔ��ˌ�(��)
   const float AllLightSpcBlue = paramMap["alllight_spcblue"];                //!< �W�������̋��ʔ��ˌ�(��)
   const float AllLightSpcAlpha = paramMap["alllight_spcalpha"];              //!< �W�������̋��ʔ��ˌ�(�����x)
   const float AllLightAmbRed = paramMap["alllight_ambred"];                  //!< �W�������̊���(��)
   const float AllLightAmbGreen = paramMap["alllight_ambgreen"];              //!< �W�������̊���(��)
   const float AllLightAmbBlue = paramMap["alllight_ambblue"];                //!< �W�������̊���(��)
   const float AllLightAmbAlpha = paramMap["alllight_ambalpha"];              //!< �W�������̊���(�����x)
   const float FixedLightArea = paramMap["fixedlight_area"];                  //!< �Œ�����̏Ǝ˔͈�
   const float FixedLightAttenFirst = paramMap["fixedlight_atten_first"];     //!< �Œ�����̋��������p�����[�^�[1(���Z�l)
   const float FixedLightAttenSecond = paramMap["fixedlight_atten_second"];   //!< �Œ�����̋��������p�����[�^�[2(���Z�l)
   const float FixedLightAttenThird= paramMap["fixedlight_atten_third"];      //!< �Œ�����̋��������p�����[�^�[3(���Z�l)
   const float FixedLightDifRed = paramMap["fixedlight_difred"];              //!< �Œ�����̊g�U���ˌ�(��)
   const float FixedLightDifGreen = paramMap["fixedlight_difgreen"];          //!< �Œ�����̊g�U���ˌ�(��)
   const float FixedLightDifBlue = paramMap["fixedlight_difblue"];            //!< �Œ�����̊g�U���ˌ�(��)
   const float FixedLightDifAlpha = paramMap["fixedlight_difalpha"];          //!< �Œ�����̊g�U���ˌ�(�����x)
   const float FixedLightSpcRed = paramMap["fixedlight_spcred"];              //!< �Œ�����̋��ʔ��ˌ�(��)
   const float FixedLightSpcGreen = paramMap["fixedlight_spcgreen"];          //!< �Œ�����̋��ʔ��ˌ�(��)
   const float FixedLightSpcBlue = paramMap["fixedlight_spcblue"];            //!< �Œ�����̋��ʔ��ˌ�(��)
   const float FixedLightSpcAlpha = paramMap["fixedlight_spcalpha"];          //!< �Œ�����̋��ʔ��ˌ�(�����x)
   const float FixedLightAmbRed = paramMap["fixedlight_ambred"];              //!< �Œ�����̊���(��)
   const float FixedLightAmbGreen = paramMap["fixedlight_ambgreen"];          //!< �Œ�����̊���(��)
   const float FixedLightAmbBlue = paramMap["fixedlight_ambblue"];            //!< �Œ�����̊���(��)
   const float FixedLightAmbAlpha = paramMap["fixedlight_ambalpha"];          //!< �Œ�����̊���(�����x)
   const int ShadowResolution = paramMap["shadow_resolution"];                //!< �����e�̉𑜓x
   // json�t�@�C������Vector4�^�̒l���擾����
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("lightshadow", { "fixedlight_pos" ,"shaodw_minarea" ,"shaodw_maxarea" });
   const auto FixedLightPos = vecParamMap["fixedlight_pos"];                  //!< �Œ�����ʒu
   const auto ShadowMinArea = vecParamMap["shaodw_minarea"];                  //!< �����e�`��͈͍ŏ��l
   const auto ShadowMaxArea = vecParamMap["shaodw_maxarea"];                  //!< �����e�`��͈͍ő�l

#ifdef _DEBUG
   constexpr auto MatDifRed = 0.0f;           //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifGreen = 0.0f;         //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifBlue = 0.0f;          //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifAlpha = 1.0f;         //!< �}�e���A���̊g�U���ˌ�(�����x)
   constexpr auto MatSpeRed = 0.0f;           //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeGreen = 0.0f;         //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeBlue = 0.0f;          //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeAlpha = 0.0f;         //!< �}�e���A���̋��ʔ��ˌ�(�����x)
   constexpr auto MatAmbRed = 0.0f;           //!< �}�e���A���̊���(��)
   constexpr auto MatAmbGreen = 0.0f;         //!< �}�e���A���̊���(��)
   constexpr auto MatAmbBlue = 0.0f;          //!< �}�e���A���̊���(��)
   constexpr auto MatAmbAlpha = 0.0f;         //!< �}�e���A���̊���(�����x)
   constexpr auto MatEmiRed = 0.0f;           //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiGreen = 0.0f;         //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiBlue = 0.5f;          //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiAlpha = 0.0f;         //!< �}�e���A���̕��ˌ�(�����x)
   constexpr auto MatPower = 20.0f;           //!< �}�e���A���̋��ʔ��ˋ��x
   constexpr auto LightSphereDiffY = 100.0;   //!< �������̂̌��������Y����
   constexpr auto LightSphereRadius = 80.0f;  //!< �������̔��a
   constexpr auto LightSphereDiv = 32;        //!< �������̕�����
   constexpr auto LightSphereRed = 0;         //!< �������̐F(��)
   constexpr auto LightSphereGreen = 128;     //!< �������̐F(��)
   constexpr auto LightSphereBlue = 200;      //!< �������̐F(��)
#endif
}

using namespace FragmentValkyria::Lighting;

LightAndShadow::LightAndShadow(Game::GameMain& gameMain) :_gameMain{gameMain} {
   namespace AppMath = AppFrame::Math;
#ifdef _DEBUG
   // �}�e���A���̃��C�e�B���O���̐ݒ�
   MATERIALPARAM material;
   material.Diffuse = GetColorF(MatDifRed, MatDifGreen, MatDifBlue, MatDifAlpha);
   material.Specular = GetColorF(MatSpeRed, MatSpeGreen, MatSpeBlue, MatSpeAlpha);
   material.Ambient = GetColorF(MatAmbRed, MatAmbGreen, MatAmbBlue, MatAmbAlpha);
   material.Emissive = GetColorF(MatEmiRed, MatEmiGreen, MatEmiBlue, MatEmiAlpha);
   material.Power = MatPower;
   SetMaterialParam(material);
#endif
   // �t�H�O��L���ɂ���
   SetFogEnable(FogEnuble);
   // �t�H�O�̐F�̐ݒ�
   SetFogColor(FogRed, FogGreen, FogBlue);
   // �t�H�O�̎n�_�ƏI�_�̐ݒ�
   SetFogStartEnd(FogStart, FogEnd);
   // �W��������L���ɂ��邩
   SetLightEnable(AllLightEnuble);
   // �S�Ẵ��f���ɓK�����������̐ݒ�
   SetGlobalAmbientLight(GetColorF(GlobalAmbRed, GlobalAmbGreen, GlobalAmbBlue, GlobalAmbAlpha));
   // �W�������̊g�U���ˌ���ݒ�
   SetLightDifColor(GetColorF(AllLightDifRed, AllLightDifGreen, AllLightDifBlue, AllLightDifAlpha));
   // �W�������̋��ʔ��ˌ���ݒ�
   SetLightSpcColor(GetColorF(AllLightSpcRed, AllLightSpcGreen, AllLightSpcBlue, AllLightSpcAlpha));
   // �W�������̊�����ݒ�
   SetLightAmbColor(GetColorF(AllLightAmbRed, AllLightAmbGreen, AllLightAmbBlue, AllLightAmbAlpha));
   // �����ʒu�̏�����
   _lightPosition = FixedLightPos;
   // �����̈ʒu�A�e���͈͋y�ы�����������ݒ肵�A�|�C���g�����𐶐��A���C�g�n���h���ɏ���ۑ�����
   _lightHandle = CreatePointLightHandle(AppMath::ToDX(_lightPosition),
      FixedLightArea, FixedLightAttenFirst, FixedLightAttenSecond, FixedLightAttenThird);
   // �����̊g�U���ˌ���ݒ�
   SetLightDifColorHandle(_lightHandle, GetColorF(FixedLightDifRed, FixedLightDifGreen, FixedLightDifBlue, FixedLightDifAlpha));
   // �����̋��ʔ��ˌ���ݒ�
   SetLightSpcColorHandle(_lightHandle, GetColorF(FixedLightSpcRed, FixedLightSpcGreen, FixedLightSpcBlue, FixedLightSpcAlpha));
   // �����̊�����ݒ�
   SetLightAmbColorHandle(_lightHandle, GetColorF(FixedLightAmbRed, FixedLightAmbGreen, FixedLightAmbBlue, FixedLightAmbAlpha));
   // �V���h�E�}�b�v�̉𑜓x��ݒ肵�V���h�E�}�b�v�𐶐��A�V���h�E�}�b�v�n���h���ɕۑ�����
   _shadowHandle = MakeShadowMap(ShadowResolution, ShadowResolution);
   auto shadowMaxArea = ShadowMaxArea;
   auto shadowMinArea = ShadowMinArea;
   // �V���h�E�}�b�v�̉e���͈͂�ݒ�
   SetShadowMapDrawArea(_shadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;
   // �v���C���[�̓��ʒu��ObjectServer����擾
   auto playerHeadPos= _gameMain.objServer().GetVecData("PlayerHeadPos");
   // �v���C���[�����W����Œ�������W�ւ̃x�N�g�������ߐ��K�����A�V���h�E�}�b�v�̌����Ƃ���
   auto shadowDirection = (playerHeadPos - _lightPosition).Normalize();
   // �V���h�E�}�b�v�̌������X�V
   SetShadowMapLightDirection(_shadowHandle, AppMath::ToDX(shadowDirection));
}

void LightAndShadow::Render() {
   // �V���h�E�}�b�v�̕`����s��
   ShadowMap_DrawSetup(_shadowHandle);
   // �V���h�E�}�b�v�`��p��ObjectServer�̕`�����
   _gameMain.objServer().Render();
   // �V���h�E�}�b�v�̕`����I������
   ShadowMap_DrawEnd();
   // �g�p����V���h�E�}�b�v�n���h���̐ݒ�
   SetUseShadowMap(0, _shadowHandle);

#ifdef _DEBUG
   //�������̂̃f�o�b�O�`��
   namespace AppMath = AppFrame::Math;
   using Utility = AppFrame::Math::Utility;
   auto spherePos = _lightPosition;
   spherePos.SetY(_lightPosition.GetY() + LightSphereDiffY);
   DrawSphere3D(AppMath::ToDX(spherePos), LightSphereRadius, LightSphereDiv,
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue),
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), TRUE);
#endif
}

void LightAndShadow::SetDifColor(float red, float green, float blue) {
   SetLightDifColorHandle(_lightHandle, GetColorF(red, green, blue, FixedLightDifAlpha));
}

void LightAndShadow::SetAmbColor(float red, float green, float blue) {
   SetLightAmbColorHandle(_lightHandle, GetColorF(red, green, blue, FixedLightDifAlpha));
}

