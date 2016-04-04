#pragma once
#include "Robot.h"

#define SERVER_SHAPE_SPHERE     0

namespace Metabot
{
    class Server
    {
        public:
            Server();
            virtual ~Server();
            void loadRobot(Robot *robot);
            void updateRobot(Robot *robot);
            void init();

            // Marker
            void updateMarker(double x, double y);
            void disableMarker();

            // Shapes
            void addShape(int id, int type, TransformMatrix matrix, std::vector<double> params=std::vector<double>());
            void updateShape(int id, TransformMatrix matrix);
            void removeShapes();

        protected:
            void *context;
            void *pub;

            void invoke(std::string command, Json::Value arguments=Json::Value(Json::nullValue));
    };
}
