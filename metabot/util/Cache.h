#pragma once

#include <mutex>
#include <functional>
#include <string>

namespace Metabot
{
    class Cache
    {
        public:
            Cache();
            void setDirectory(std::string directory);
            int clear();
            int files();

            std::string get(std::string key, std::function<std::string(void)> generate, std::string older="");

            std::string directory;

        protected:
            std::mutex mutex;
    };
}
