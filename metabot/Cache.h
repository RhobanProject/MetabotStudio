#pragma once

#include <functional>
#include <string>

namespace Metabot
{
    class Cache
    {
        public:
            Cache();
            void setDirectory(std::string directory);

            std::string get(std::string key, std::function<std::string(void)> generate);

            std::string directory;
    };
}