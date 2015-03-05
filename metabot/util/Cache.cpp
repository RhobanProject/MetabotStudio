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

    int Cache::clear()
    {
        auto listing = get_directory_listing(directory);
        for (auto entry : listing) {
            std::string filename = directory + "/" + entry;
            remove_file(filename);
        }
        return listing.size();
    }

    int Cache::files()
    {
        return get_directory_listing(directory).size();
    }

    std::string Cache::get(std::string key, std::function<std::string(void)> generate, std::string older)
    {
        std::string filename = directory + "/" + key;

        if (!file_exists(filename) || older=="" || filemtime(filename) < filemtime(older)) {
            std::string data = generate();
            file_put_contents(filename, data);
        }
 
        return file_get_contents(filename);
    }
}
