#pragma once

#include <sstream>
#include <string>
#include <stdint.h>

namespace Metabot
{
    class Bin
    {
        public:
            Bin();
            Bin(std::string s);

            void appendInt(uint32_t i);
            void appendFloat(float f);
            std::string get();

            uint32_t readInt();
            float readFloat();

        protected:
            std::stringstream stream;
            std::string data;
            int pos;
    };
}
