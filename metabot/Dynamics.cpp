#include <iostream>
#include <sstream>
#include <json/json.h>
#include "util/Bin.h"
#include "Dynamics.h"

namespace Metabot
{
    Dynamics::Dynamics()
        : com(0, 0, 0), volume(0), mass(0), computed(false),
          ixx(0), ixy(0), ixz(0), iyy(0), iyz(0), izz(0)
    {
    }
            
    void Dynamics::combine(const Dynamics &other, TransformMatrix matrix)
    {
        if (computed) {
            if (other.computed) {
                auto other_com = matrix.apply(other.com);
                double a = mass;
                double b = other.mass;
                double total = a+b;
                if (total > 0.0) {
                    com.values[0] = (com.x()*a + other_com.x()*b)/total;
                    com.values[1] = (com.y()*a + other_com.y()*b)/total;
                    com.values[2] = (com.z()*a + other_com.z()*b)/total;
                }
                volume = volume+other.volume;
                mass = total;
                computed = true;

            }
        } else {
            if (other.computed) {
                computed = true;
                com = matrix.apply(other.com);
                mass = other.mass;
                volume = other.volume;
            }
        }
        for (auto cube : other.cubes) {
            cube.pos = matrix.apply(cube.pos);
            cubes.push_back(cube);
        }
        updateInertia();
    }

    std::string Dynamics::toString()
    {
        std::stringstream ss;
        if (computed) {
            ss << "Voxels: " << cubes.size() << std::endl;
            ss << "Volume: " << volume << "mm^3" << std::endl;
            ss << "Mass: " << mass << "g" << std::endl;
            ss << "COM: " << com.x() << ", " << com.y() << ", " << com.z() << std::endl;
            ss << "Inertia: ixx=" << ixx << ", iyy=" << iyy << ", izz=" << izz << 
                ", ixy=" << ixy << ", iyz=" << iyz << ", ixz=" << ixz << std::endl;
        } else {
            ss << "No dynamics (no geometry or not computed)" << std::endl;
        }

        return ss.str();
    }
            
    void Dynamics::updateInertia()
    {
        ixx = iyy = izz = ixy = iyz = ixz = 0;

        for (auto &cube : cubes) {
            double X = cube.pos.x() - com.x();
            double Y = cube.pos.y() - com.y();
            double Z = cube.pos.z() - com.z();
            double m = cube.mass;

            ixx += (Y*Y + Z*Z)*m;
            iyy += (X*X + Z*Z)*m;
            izz += (X*X + Y*Y)*m;
            ixy -= X*Y*m;
            iyz -= Y*Z*m;
            ixz -= X*Z*m;
        }
    }
            
    std::string Dynamics::serialize()
    {
        Metabot::Bin bin;

        bin.appendInt(computed);
        bin.appendFloat(volume);
        bin.appendFloat(mass);
        bin.appendFloat(com.values[0]);
        bin.appendFloat(com.values[1]);
        bin.appendFloat(com.values[2]);

        bin.appendInt(cubes.size());

        int k = 0;
        for (auto cube : cubes) {
            bin.appendFloat(cube.pos.values[0]);
            bin.appendFloat(cube.pos.values[1]);
            bin.appendFloat(cube.pos.values[2]);
            bin.appendFloat(cube.mass);
            k++;
        }

        return bin.get();
    }
    
    Dynamics Dynamics::unserialize(std::string data)
    {
        Dynamics dyn;
        Metabot::Bin bin(data);

        dyn.computed = bin.readInt();
        dyn.volume = bin.readFloat();
        dyn.mass = bin.readFloat();
        dyn.com.values[0] = bin.readFloat();
        dyn.com.values[1] = bin.readFloat();
        dyn.com.values[2] = bin.readFloat();

        dyn.cubes.resize(bin.readInt());
        for (int k=0; k<dyn.cubes.size(); k++) {
            dyn.cubes[k].pos.values[0] = bin.readFloat();
            dyn.cubes[k].pos.values[1] = bin.readFloat();
            dyn.cubes[k].pos.values[2] = bin.readFloat();
            dyn.cubes[k].mass = bin.readFloat();
        }

        return dyn;
    }
}
