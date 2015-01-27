#pragma once

#include <string>
#include <map>
#include "Backend.h"
#include "Parameters.h"

namespace Metabot
{
    class Module
    {
        public:
            Module();
            virtual ~Module();

            void setBackend(Backend *backend);

            void setFilename(std::string filename);
            std::string getFilename();

            void setName(std::string name);
            std::string getName();

            void setPrettyName(std::string prettyName);
            std::string getPrettyName();
            
            void setType(std::string type);
            std::string getType();

            void setDescription(std::string description);
            std::string getDescription();

            void setColor(std::string color);

            Parameters &getParameters();
            Parameter &getParameter(std::string name);
            bool hasParameter(std::string name);

            std::string openscad(std::string format, Parameters parameters=Parameters(), bool noModels=false, bool collisions=false);
            std::string doOpenscad(std::string format, Parameters parameters=Parameters(), bool noModels=false, bool collisions=false);
            
            // The module color
            float r, g, b;
            // Module mass & density
            float mass, density;

        protected:
            // Backend
            Backend *backend;
            std::string filename, name, type, description, prettyName;
            // Module parameters, default values & descriptions
            Parameters parameters; 
    };
}
