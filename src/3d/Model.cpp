#include <iostream>
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

    void Model::translate(float X, float Y, float Z)
    {
        for (auto& volume : volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    face.v[i].x += X;
                    face.v[i].y += Y;
                    face.v[i].z += Z;
                }
            }
        }
    }
        
    void Model::merge(const Model &other)
    {
        for (auto volume : other.volumes) {
            volumes.push_back(volume);
        }
    }
    
    void Model::rotateZ(float r)
    {
        for (auto& volume : volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float y = face.v[i].y;
                    face.v[i].x = cos(r)*x-sin(r)*y;
                    face.v[i].y = sin(r)*x+cos(r)*y;
                }
            }
        }
    }
        
    void Model::rotateY(float r)
    {
        for (auto& volume : volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float z = face.v[i].z;
                    face.v[i].x = cos(r)*x-sin(r)*z;
                    face.v[i].z = sin(r)*x+cos(r)*z;
                }
            }
        }
    }
        
    void Model::rotateX(float r)
    {
        for (auto& volume : volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float y = face.v[i].y;
                    float z = face.v[i].z;
                    face.v[i].y = cos(r)*y-sin(r)*z;
                    face.v[i].z = sin(r)*y+cos(r)*z;
                }
            }
        }
    }
            
    void Model::apply(TransformMatrix matrix)
    {
        for (auto& volume : volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    Vector pt(face.v[i].x, face.v[i].y, face.v[i].z);
                    // std::cout << pt.toString() << std::endl;
                    pt = matrix.apply(pt);
                    // std::cout << "=>" << std::endl;
                    // std::cout << pt.toString() << std::endl;
                    face.v[i].x = pt.values[0];
                    face.v[i].y = pt.values[1];
                    face.v[i].z = pt.values[2];
                }
            }
        }
    }
}
