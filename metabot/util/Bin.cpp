#include <stdexcept>
#include <sstream>
#include "Bin.h"

namespace Metabot
{
    Bin::Bin()
        : pos(0)
    {
    }

    Bin::Bin(std::string data)
        : data(data), pos(0)
    {
    }
    void Bin::appendInt(uint32_t i)
    {
        stream.write(reinterpret_cast<const char*>(&i), sizeof(uint32_t));
    }

    void Bin::appendFloat(float f)
    {
        stream.write(reinterpret_cast<const char*>(&f), sizeof(float));
    }
            
    std::string Bin::get()
    {
        return stream.str();
    }

    uint32_t Bin::readInt()
    {
        if (pos+sizeof(float) > data.size()) {
            throw std::runtime_error("Bin: trying to read too much");
        } else {
            const char *c = data.c_str()+pos;
            const uint32_t f = *reinterpret_cast<const uint32_t*>(c);
            pos += sizeof(float);

            return f;
        }
    }

    float Bin::readFloat()
    {
        if (pos+sizeof(float) > data.size()) {
            throw std::runtime_error("Bin: trying to read too much");
        } else {
            const char *c = data.c_str()+pos;
            const float f = *reinterpret_cast<const float*>(c);
            pos += sizeof(float);

            return f;
        }
    }
}
