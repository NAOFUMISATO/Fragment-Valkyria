#pragma once
/*****************************************************************//**
 * \file   CameraComponent.h
 * \brief  �J�����Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include <tuple>
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
    namespace Game {
        class GameMain;
    }
   /**
    * \brief �J�����֌W
    */
   namespace Camera {
      /**
       * \class �J�����Ǘ��N���X
       * \brief �C���Q�[���Ŏg�p����J�������Ǘ�����
       */
      class CameraComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using InputManager = AppFrame::Input::InputManager;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         CameraComponent(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init();
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Draw();

         /**
          * \brief �J�����̈ʒu��ݒ肷��
          * \param position �J�����̈ʒu
          */
         void SetPosition(Vector4 position) { _position = position; }

         void SetPlyPos(Vector4 plyPos) { _plyPos = plyPos; }
         /**
          * \brief �J�����̒����_��ݒ肷��
          * \param target �����_�ɂȂ���W
          * \param forward �J�����̌���
          */
         inline void SetTarget(Vector4 target) {
             _target = target;
         }

         inline Vector4 GetPos() const { return _position; }

         inline Vector4 GetTarget() const { return _target; }
         /**
          * \brief �����_�����̃x�N�g���̎擾
          * \return �����_�����̒P�ʃx�N�g��
          */
         Vector4 GetForward() const {
            auto vec = _target - _position;
            vec.Normalized();
            return vec;
         }
         
         void stateServer(std::unique_ptr<StateServer> state) {
             _stateServer = std::move(state);
         }

         inline StateServer& stateServer() { return *_stateServer; }

         void SetZoom(bool zoom) { _zoom = zoom; }

      private:
         /**
          * \brief �J�����̃r���[�s��̐ݒ�
          * \param cameraPosition �J�����̈ʒu
          * \param cameraTarget �J�����̒����_
          * \param cameraUp �J�����̏����
          * \return �J�����̃r���[�s��
          */
         Matrix44 GetCameraViewMatrix(Vector4& cameraPosition, Vector4& cameraTarget, Vector4& cameraUp);
         /**
          * \brief �J�����̓��e�s��̐ݒ�
          * \param cameraNear �J�����̕`����E(��O)
          * \param cameraFar �J�����̕`����E(��)
          * \param fov ����p
          * \return �J�����̓��e�s��
          */
         Matrix44 GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov);

         void Rotate();

         void Placement();
         bool _zoom{ false };
         Vector4 _position{ 0, 0, 0 };        //!< �ʒu
         Vector4 _target{ 0, 0, 0 };          //!< �����_
         Vector4 _up{ 0, 1 ,0 };              //!< �����
         Vector4 _forwardOfTarget{ 0, 0, 1}; //!< �����_�I�u�W�F�N�g�̌���
         Vector4 _plyToPos{ 0, 0, 0 };
         Vector4 _plyToTarget{ 0, 0, 0 };
         Vector4 _firstPlyToPos{ 0, 0, 0 };
         Vector4 _firstPlyToTarget{ 0, 0, 0 };
         Vector4 _posToTarget{ Vector4(0.0, 0.0, 0.0) };
         Vector4 _zoomRate{ Vector4(0.0, 0.0, 0.0) };
         Vector4 _plyPos{ 0, 0, 0 };
         Matrix44 _rotateMatrix{ Matrix44() };
         Matrix44 _anyAxisMatrix{ Matrix44() };
         Game::GameMain& _gameMain;
         double _targetDistance{ 500 };       //!< �����_�I�u�W�F�N�g�Ƃ�Z���W�̋���
         double _vertDistance{ 120 };         //!< �����_�I�u�W�F�N�g�Ƃ�Y���W�̋���
         double _zoomRateRadian{ 0.0 };
         double _upDownAngle{ 0.0 };
         double _sideAngle{ 0.0 };
         std::tuple<double, double, double> _nearFarFov{   //!< �J�����̕`����E(��O,��)�y�ю���p��Tuple�^(�����ϊ��Ɏg�p)
            std::make_tuple(50.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(60.0)) };

         std::unique_ptr<StateServer> _stateServer;                        //!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^

      public:
          /**
          * \class �J�����̏�Ԃ̊��N���X
          * \brief �e�J�����̏�Ԃ͂����h�����Ē�`����
          */
          class StateBase : public AppFrame::State::StateBaseRoot {
          public:
              /**
              * \brief �R���X�g���N�^
              * \param owner �J�����̎Q��
              */
              StateBase(CameraComponent& owner) : _owner{ owner } {};
              /**
               * \brief �`�揈��
               */
              void Draw() override;

          protected:
              CameraComponent& _owner;   //!< �J�����̎Q��
          };
          /**
          * \class �ʏ��ԃN���X
          * \brief �ʏ��Ԃ̏�������
          */
          class StateNormal : public StateBase
          {
          public:
              /**
               * \brief �R���X�g���N�^
               * \param owner �J�����̎Q��
               */
              StateNormal(CameraComponent& owner) : StateBase{ owner } {};
              /**
               * \brief ��������
               */
              void Enter() override;
              /**
               * \brief ���͏���
               * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
               */
              void Input(InputManager& input) override;
              /**
               * \brief �X�V����
               */
              void Update() override;
          };
          /**
          * \class �Y�[���C����ԃN���X
          * \brief �Y�[���C����Ԃ̏�������
          */
          class StateZoomIn : public StateBase
          {
          public:
              /**
               * \brief �R���X�g���N�^
               * \param owner �J�����̎Q��
               */
              StateZoomIn(CameraComponent& owner) : StateBase{ owner } {};
              /**
               * \brief ��������
               */
              void Enter() override;
              /**
               * \brief ���͏���
               * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
               */
              void Input(InputManager& input) override;
              /**
               * \brief �X�V����
               */
              void Update() override;
          };
          /**
         * \class �ˌ�������ԃN���X
         * \brief �ˌ�������Ԃ̏�������
         */
          class StateShootReady : public StateBase
          {
          public:
              /**
               * \brief �R���X�g���N�^
               * \param owner �J�����̎Q��
               */
              StateShootReady(CameraComponent& owner) : StateBase{ owner } {};
              /**
               * \brief ��������
               */
              void Enter() override;
              /**
               * \brief ���͏���
               * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
               */
              void Input(InputManager& input) override;
              /**
               * \brief �X�V����
               */
              void Update() override;
          };
          /**
          * \class �Y�[���A�E�g��ԃN���X
          * \brief �Y�[���A�E�g��Ԃ̏�������
          */
          class StateZoomOut : public StateBase
          {
          public:
              /**
               * \brief �R���X�g���N�^
               * \param owner �J�����̎Q��
               */
              StateZoomOut(CameraComponent& owner) : StateBase{ owner } {};
              /**
               * \brief ��������
               */
              void Enter() override;
              /**
               * \brief ���͏���
               * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
               */
              void Input(InputManager& input) override;
              /**
               * \brief �X�V����
               */
              void Update() override;
          };
      };
   }
}