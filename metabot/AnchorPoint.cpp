#include <math.h>
#include <iostream>
#include <sstream>
#include "Backend.h"
#include "AnchorPoint.h"
#include "Component.h"

namespace Metabot
{
    AnchorPoint::AnchorPoint(Json::Value json, TransformMatrix matrix_)
        : type(""), matrix(matrix_), anchor(NULL), above(true), alpha(0.0), zero(0.0), 
        orientationX(0), orientationY(0), orientationZ(0), cached(false)
    {
        if (json.isObject()) {
            type = json["type"].asString();
            male = json["male"].asBool();
            female = json["female"].asBool();
        }

#ifdef OPENGL
        highlight = false;
        hover = false;
#endif
    }
            
    AnchorPoint::AnchorPoint(std::string type_, bool male_, bool female_, TransformMatrix matrix_, float zero_)
        : type(type_), male(male_), female(female_), matrix(matrix_), anchor(NULL), above(true), alpha(0.0), zero(zero_)
    {
    }

    AnchorPoint::~AnchorPoint()
    {
        bool iAmAbove = above;
        above = false;

        if (anchor != NULL && iAmAbove) {
            if (anchor->component != NULL) {
                delete anchor->component;
            }
            delete anchor;
        }
    }
            
    AnchorPoint *AnchorPoint::clone()
    {
        AnchorPoint *anchorPoint = new AnchorPoint(type, male, female, matrix, zero);
        anchorPoint->above = above;
        anchorPoint->copyData(this);

        return anchorPoint;
    }
            
    void AnchorPoint::copyData(AnchorPoint *other)
    {
        orientationX = other->orientationX;
        orientationY = other->orientationY;
        orientationZ = other->orientationZ;
        alpha = other->alpha;
        zero = other->zero;
        computeMatrixes();
    }
            
    void AnchorPoint::revert()
    {
        if (anchor) {
            anchor->component->root();
            above = true;
            orientationX = anchor->orientationX;
            orientationY = anchor->orientationY;
            orientationZ = anchor->orientationZ;
            anchor->above = false;
            float tmp = anchor->zero;
            anchor->zero = -zero;
            zero = -tmp;
            computeMatrixes();
        }
    }

    bool AnchorPoint::isCompatible(AnchorPoint *anchor)
    {
        if (anchor != NULL && type == anchor->type) {
            return (male && anchor->female)
                || (female && anchor->male);
        }

        return false;
    }

    void AnchorPoint::attach(AnchorPoint *anchor_, bool above_)
    {
        above = above_;

        if (!isCompatible(anchor_)) {
            anchor = NULL;
            /*
               std::stringstream ss;
               if (anchor_ == NULL) {
               ss << "Can't attach anchor (root) on an anchor " << type;
               } else {
               ss << "Can't attach anchor " << anchor_->type << " on an anchor " << type;
               }
               throw ss.str();
               */
            return;
        }
        anchor = anchor_;

        if (above) {
            anchor->attach(this, false);
        }
    }

    void AnchorPoint::detach(bool remove)
    {
        if (anchor != NULL) {
            if (remove) {
                delete anchor;
            }
            anchor = NULL;
            above = true;
        }
    }

    Model AnchorPoint::toModel()
    {
        Model m;

        if (above) {
            if (anchor != NULL) {
                m = anchor->toModel();
                m.apply(transformationForward());
            }
        } else {
            if (component != NULL) {
                m = component->toModel();
                m.apply(transformationBackward());
            }
        }

        return m;
    }

    void AnchorPoint::computeMatrixes()
    {
        {
            TransformMatrix rotation = TransformMatrix::identity();
            if (orientationX) rotation = rotation.multiply(TransformMatrix::rotationX(orientationX));
            if (orientationY) rotation = rotation.multiply(TransformMatrix::rotationY(orientationY));
            if (orientationZ) rotation = rotation.multiply(TransformMatrix::rotationZ(orientationZ));
            rotation = rotation.multiply(TransformMatrix::rotationZ(zero+alpha));
            forwardAbove = matrix.multiply(rotation);
            backwardAbove = forwardAbove.invert();
        }
        {
            forward = matrix;
            backward = forward.invert();
        }
            
        cached = true;
    }

    TransformMatrix AnchorPoint::transformationForward()
    {
        if (!cached) {
            computeMatrixes();
        }

        return above ? forwardAbove : forward;
    }
    
    TransformMatrix AnchorPoint::transformationBackward()
    {
        if (!cached) {
            computeMatrixes();
        }

        return above ? backwardAbove : backward;
    }

#ifdef OPENGL
    void AnchorPoint::openGLDraw(int id)
    {
        glStencilFunc(GL_ALWAYS, 200+id, -1);

        if (anchor != NULL) {
            if (above) {
                transformationForward().openGLMult();
                anchor->openGLDraw();
            } else {
                transformationBackward().openGLMult();
                component->openGLDraw();
            }
        } else {
            if (above && (highlight || hover)) {
                matrix.openGLMult();
                Model m = component->backend->getModel("anchor");
                m.r = 0.4;
                m.g = 0.3;
                m.b = 1.0;
                m.openGLDraw();
            }
        }
    }
#endif
            
    void AnchorPoint::writeURDF(std::stringstream &ss, std::string parent)
    {
        std::stringstream tmp;
        tmp << "anchor_" << component->id << "_" << id;
        auto name = tmp.str();

        // Creating anchor link
        ss << "  <link name=\"" << name<< "\">" << std::endl;
        ss << "  <visual><origin xyz=\"0 0 0\" rpy=\"0 0 0\"/><geometry><box size=\"0.0001 0.0001 0.0001\"/></geometry></visual>" << std::endl;
        ss << "  </link>" << std::endl;
        auto matrix = above ? transformationForward() : transformationBackward();
        ss << "  <joint name=\"" << name << "_joint\" type=\"fixed\">" << std::endl;
        ss << "    <parent link=\"" << parent << "\"/>" << std::endl;
        ss << "    <child link=\"" << name << "\"/>" << std::endl;
        ss << "    " << matrix.toURDF() << std::endl;
        ss << "  </joint>" << std::endl;

        if (above && anchor) {
            anchor->writeURDF(ss, name);
        }
        if (!above) {
            component->writeURDF(ss, name);
        }
    }
}
