#pragma once
/*****************************************************************//**
 * \file   CollisionServer.h
 * \brief  ìñÇΩÇËîªíËÇçsÇ§ÉNÉâÉX
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"

namespace FragmentValkyria {

	namespace Object {
		class ObjectBase;
	}

	namespace Collision {

		class CollisionComponent {
			using Vector4 = AppFrame::Math::Vector4;
		public:
			enum class ReportId {
				None,
				HitFromPlayer,
				HitFromObjectRange,
				HitFromIdleFallObject,
				HitFromFallObject,
				HitFromGatling,
				HitFromLargeEnemy
			};

			class Report {
			public:

				void id(ReportId id) { _id = id; }
				ReportId& id() { return _id; }

			private:
				ReportId _id{ ReportId::None };

			};

			CollisionComponent(Object::ObjectBase& owner);

			void ObjectRangeFromPlayer();

			void PlayerFromObjectRange();

			void PlayerFromFallObjectModel(bool fall);

			void GatlingFromObjectModel();

			void GatlingFromPlayer();

			void ObjectModelFromLargeEnemy();

			void report(Report report) { *_report = report; }
			Report& report() { return *_report; }

			void hitPos(Vector4 pos) { _hitPos = pos; }

			Vector4 hitPos() { return _hitPos; }

			void damage(double damage) { _damage = damage; }

			double damage() { return _damage; }
		private:

			Object::ObjectBase& ObjectBaseCast(AppFrame::Object::ObjectBaseRoot& obj);

			Object::ObjectBase& _owner;
			std::unique_ptr<Report> _report;
			double _damage{ 0.0 };

			Vector4 _hitPos;

		};

	}
}
