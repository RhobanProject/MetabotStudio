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
        orientationX(0), orientationY(0), orientationZ(0)
    {
        if (json.isObject()) {
            type = json["type"].asString();
            male = json["male"].asBool();
            female = json["female"].asBool();
        }

#ifdef OPENGL
        highlight = false;
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
            if (anchor->instance != NULL) {
                delete anchor->instance;
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
            anchor->instance->root();
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
                if (orientationX) m.rotateX(-orientationX);
                if (orientationY) m.rotateY(-orientationY);
                if (orientationZ) m.rotateZ(-orientationZ);
                m.rotateZ(-zero-alpha);
                m.apply(matrix);
            }
        } else {
            if (instance != NULL) {
                m = instance->toModel();
                m.apply(matrix.invert());
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
    void AnchorPoint::openGLDraw()
    {
        if (anchor != NULL) {
            if (above) {
                transformationForward().openGLMult();
                anchor->openGLDraw();
            } else {
                transformationBackward().openGLMult();
                instance->openGLDraw();
            }
        } else {
            if (above && highlight) {
                matrix.openGLMult();
                Model m = instance->component->backend->getModel("anchor");
                m.r = 0.4;
                m.g = 0.3;
                m.b = 1.0;
                m.openGLDraw();
            }
        }
    }
#endif
}
