#include <math.h>
#include "Model.h"

namespace Metabot
{

    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    Point3 Model::min(){
    	if (volumes.size() < 1)
            return Point3(0, 0, 0);

        Point3 min = volumes[0].min();

        int xmin = min.x;
        int ymin = min.y;
        int zmin = min.z;

        for (auto v : volumes) {
            Point3 p = v.min();
            if(p.x < xmin)
            	xmin = p.x;
            if(p.y < ymin)
            	ymin = p.y;
            if(p.z < zmin)
            	zmin = p.z;
        }
        return Point3(xmin, ymin, zmin);
    }

    Point3 Model::max(){
    	if (volumes.size() < 1)
            return Point3(0, 0, 0);

        Point3 max = volumes[0].max();

        int xmax = max.x;
        int ymax = max.y;
        int zmax = max.z;

        for (auto v : volumes) {
            Point3 p = v.max();
            if(p.x > xmax)
            	xmax = p.x;
            if(p.y > ymax)
            	ymax = p.y;
            if(p.z > zmax)
            	zmax = p.z;
        }
        return Point3(xmax, ymax, zmax);
    }

    Model Model::translate(float X, float Y, float Z)
    {
        Model translated;
        translated.volumes = volumes;

        for (auto& volume : translated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    face.v[i].x += X;
                    face.v[i].y += Y;
                    face.v[i].z += Z;
                }
            }
        }

        return translated;
    }
        
    void Model::merge(const Model &other)
    {
        for (auto volume : other.volumes) {
            volumes.push_back(volume);
        }
    }
    
    Model Model::rotateZ(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float y = face.v[i].y;
                    face.v[i].x = cos(r)*x-sin(r)*y;
                    face.v[i].y = sin(r)*x+cos(r)*y;
                }
            }
        }

        return rotated;
    }
        
    Model Model::rotateY(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float z = face.v[i].z;
                    face.v[i].x = cos(r)*x-sin(r)*z;
                    face.v[i].z = sin(r)*x+cos(r)*z;
                }
            }
        }

        return rotated;
    }
        
    Model Model::rotateX(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float y = face.v[i].y;
                    float z = face.v[i].z;
                    face.v[i].y = cos(r)*y-sin(r)*z;
                    face.v[i].z = sin(r)*y+cos(r)*z;
                }
            }
        }

        return rotated;
    }
}
