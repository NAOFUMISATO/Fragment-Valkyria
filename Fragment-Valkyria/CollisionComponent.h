#pragma once
/*****************************************************************//**
 * \file   CollisionServer.h
 * \brief  
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
		public:
			enum class ReportId {
				None,
				HitFromPlayer,
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

			void report(Report report) { *_report = report; }
			Report& report() { return *_report; }
		private:

			Object::ObjectBase& _owner;
			std::unique_ptr<Report> _report;

		};

	}
}
