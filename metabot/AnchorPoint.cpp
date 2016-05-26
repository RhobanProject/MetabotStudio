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
        orientationX(0), orientationY(0), orientationZ(0), cached(false),
        minimum(-M_PI), maximum(M_PI), inverted(false)
    {
        if (json.isObject()) {
            type = json["name"].asString();
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

    int AnchorPoint::sign()
    {
        int s = male ? 1 : -1;
        if (inverted) s *= -1;

        return s;
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
        inverted = other->inverted;
        minimum = other->minimum;
        maximum = other->maximum;
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
            zero = anchor->zero;
            anchor->zero = 0;
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

    TransformMatrix AnchorPoint::orientationMatrix()
    {
        TransformMatrix rotation = TransformMatrix::identity();
        if (orientationX) rotation = rotation.multiply(TransformMatrix::rotationX(orientationX));
        if (orientationY) rotation = rotation.multiply(TransformMatrix::rotationY(orientationY));
        if (orientationZ) rotation = rotation.multiply(TransformMatrix::rotationZ(orientationZ));

        return rotation;
    }
            
    void AnchorPoint::setZero(float zero_)
    {
        zero = zero_;
        if (zero < minimum) zero = minimum;
        if (zero > maximum) zero = maximum;

        computeMatrixes();
    }

    void AnchorPoint::computeMatrixes()
    {
        {
            auto orientation = orientationMatrix();
            auto rotation = TransformMatrix::rotationZ(sign()*(zero+alpha));
            forwardAbove = matrix.multiply(orientation.multiply(rotation));
            backwardAbove = forwardAbove.invert();
        }
        {
            forward = matrix;
            backward = forward.invert();
        }
            
        cached = true;
    }

    Symbolic AnchorPoint::symbolicTransformation(std::string name)
    {
        //Symbolic cos_a("cos_"+name);
        //Symbolic sin_a("sin_"+name);
        Symbolic a(name);
        auto save_alpha = alpha;
        alpha = 0;
        computeMatrixes();
        Symbolic transformation = transformationForward().toSymbolic();
        Symbolic rotation("r", 4, 4);
        rotation = rotation.identity();
        //rotation(0,0) = cos_a;
        //rotation(0,1) = -sin_a;
        //rotation(1,0) = sin_a;
        //rotation(1,1) = cos_a;
        
        rotation(0,0) = cos(a);
        rotation(0,1) = -sin(a);
        rotation(1,0) = sin(a);
        rotation(1,1) = cos(a);

        alpha = save_alpha;
        computeMatrixes();

        return transformation*rotation;
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
    void AnchorPoint::openGLDraw(int id, bool bullet, bool collisions, float alpha)
    {
        glStencilFunc(GL_ALWAYS, 200+id, -1);

        if (anchor != NULL) {
            if (above) {
                if (!bullet) transformationForward().openGLMult();
                anchor->openGLDraw(1, bullet, collisions, alpha);
            } else {
                if (!bullet) transformationBackward().openGLMult();
                component->openGLDraw(bullet, collisions, alpha);
            }
        } else {
            if (above && (highlight || hover)) {
                matrix.openGLMult();
                auto m = component->backend->getModel("anchor");
                m.r = 0.4;
                m.g = 0.3;
                m.b = 1.0;
                m.openGLDraw(alpha);
            }
        }
    }
#endif
}
