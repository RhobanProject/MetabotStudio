#pragma once

#include <string>
#include <map>
#include "Cache.h"

namespace Metabot
{
    class Module
    {
        public:
            Module();
            Module(std::string file);

            void setCache(Cache *cache);

            std::string getFilename();

            void setName(std::string name);
            std::string getName();
            
            void setType(std::string type);
            std::string getType();

            void setDescription(std::string description);
            std::string getDescription();

            /**
             * Pushes a new line of module header
             */
            std::string pushLine(std::string line);

            /**
             * Pushes a new character in the buffer
             */
            std::string push(unsigned char c);

            bool finished();

        protected:
            std::string file, name, type, description;
            std::map<std::string, std::string> parameters;
            int state;

            Cache *cache;
        
            int brackets, equals;
            std::string tmpName, tmpValue;
    };
}
