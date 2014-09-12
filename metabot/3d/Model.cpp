#include <iostream>
#include <math.h>
#include "Model.h"
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif

namespace Metabot
{

    Model::Model()
        : r(1.0), g(1.0), b(1.0)
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

#ifdef OPENGL
    void Model::openGLDraw(float brightness)
    {
        glColor3ub(r*brightness*255, g*brightness*255, b*brightness*255);
        glBegin(GL_TRIANGLES);
        for (auto &volume : volumes) {
            for (auto &face : volume.faces) {
                float x1 = face.v[1].x-face.v[0].x;
                float y1 = face.v[1].y-face.v[0].y;
                float z1 = face.v[1].z-face.v[0].z;

                float x2 = face.v[2].x-face.v[0].x;
                float y2 = face.v[2].y-face.v[0].y;
                float z2 = face.v[2].z-face.v[0].z;

                float Nx = (y1*z2 - z1*y2);
                float Ny = (z1*x2 - x1*z2);
                float Nz = (x1*y2 - y1*x2);
                float Nd = sqrt(Nx*Nx+Ny*Ny+Nz*Nz);

                glNormal3f(Nx/Nd, Ny/Nd, Nz/Nd);
                for (int i=0; i<3; i++) {
                    glVertex3f(face.v[i].x, face.v[i].y, face.v[i].z);
                }
            }
        }
        glEnd();
    }
#endif
}
