#include <iostream>
#include "Cache.h"
#include "util.h"
            
namespace Metabot
{
    Cache::Cache()
    {
    }
            
    void Cache::setDirectory(std::string directory_)
    {
        directory = directory_;
        if (!is_directory(directory)) {
            makedir(directory);
        }
    }

    std::string Cache::get(std::string key, std::function<std::string(void)> generate)
    {
        std::string filename = directory + "/" + key;

        if (!file_exists(filename)) {
            std::string data = generate();
            file_put_contents(filename, data);
        }
        
        return file_get_contents(filename);
    }
}
