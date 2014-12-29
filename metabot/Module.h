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
            Module(std::string file);

            void setBackend(Backend *backend);

            std::string getFilename();

            void setName(std::string name);
            std::string getName();
            
            void setType(std::string type);
            std::string getType();

            void setDescription(std::string description);
            std::string getDescription();

            Parameter &getParameter(std::string name);

            /**
             * Pushes a new line of module header
             */
            std::string pushLine(std::string line);

            /**
             * Pushes a new character in the buffer
             */
            std::string push(unsigned char c);

            bool finished();
            
            std::string openscad(std::string format, Parameters parameters, bool noModels=false);
            std::string doOpenscad(std::string format, Parameters parameters, bool noModels=false);

        protected:
            std::string filename, name, type, description;
            Parameters parameters;
            int state;

            Backend *backend;
        
            int brackets, equals;
            std::string tmpName, tmpValue;
    };
}
