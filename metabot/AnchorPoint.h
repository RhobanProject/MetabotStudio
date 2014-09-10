#pragma once

#include <string>
#include <3d/Model.h>
#include "TransformMatrix.h"
#include "ComponentInstance.h"

namespace Metabot
{
    class Component;
    class AnchorPoint
    {
        public:
            AnchorPoint(std::string name, TransformMatrix matrix);
            virtual ~AnchorPoint();
            
            void attach(ComponentInstance *instance);

            Model toModel();

            std::string type;
            std::string model;
            TransformMatrix matrix;
            ComponentInstance *instance;

            float alpha;
    };
}
