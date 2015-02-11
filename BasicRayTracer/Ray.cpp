#include "Ray.h"
#include "Sphere.h"
extern SceneIO *scene;
extern std::vector<Primitive*> objects;
extern std::vector<LightIO*> lights;
size_t Ray::counter = 0;
Ray::Ray(Pos startPosition, Vec3f direction)
:_id(++counter), startPosition(startPosition), direction(direction.normalize())
{
    t_max = INFINITY;
    color = BACKGROUND_COLOR;
}

Pos Ray::intersectionPoint() const {
    return startPosition + direction * t_max;
}


Colr Ray::trace(int bounces){
    if(bounces <= 0){return Colr(0,0,0);}

    // Find which object we intersect closest:
    for ( Primitive* object : objects ) {
        object->intersect(*this);
    }

    if(t_max == INFINITY){ // No hit.
        return BACKGROUND_COLOR;
    }

    Colr ambientColor = ambient();

    /* Per light stuff: diffuse, specular, shadow: */
    Colr shadowFactor, diffuseColor, specularColor;
    for (LightIO* light: lights){
        shadowFactor = shadow(light);
        diffuseColor += diffuse(light) * shadowFactor;
        specularColor += specular(light) * shadowFactor;
    }

    Colr reflectionColor = Colr(0,0,0);
    if(isReflective()) {
        reflectionColor = reflection(intersectionPoint(), bounces-1) * Colr(material.specColor);
    }

    Colr refractionColor = Colr(0,0,0);;
    if(material.ktran > 0.001){
        refractionColor = refraction(intersectionPoint(), bounces-1) * material.ktran;
    }

    Colr result = ambientColor + diffuseColor + specularColor + reflectionColor + refractionColor;
    result.capColor();
    return result;
}

bool Ray::isReflective() const {
    return (material.specColor[0] > 0.0
            && material.specColor[1] > 0.0
            && material.specColor[2] > 0.0);
}

Colr Ray::reflection(const Pos point, const int bounces) const {
    Vec3f incident = direction * -1.0;
    Vec3f R = intersectionNormal* 2.0 * Vec3f::dot(incident, intersectionNormal)  - incident;
    Ray reflectionRay = Ray(point, R);
    Colr reflectionColor = reflectionRay.trace(bounces);
    return reflectionColor;

}

Colr Ray::refraction(const Pos point, const int bounces){
    // Keep track of which objects we have crossed into, for refraction rays etc..
    // Are we currently inside an object?
    bool isInside = insideObjects.size() > 0;
    bool enteringCurrentObject = insideObjects.find(currentObject) == insideObjects.end();

    if(!enteringCurrentObject){ intersectionNormal = intersectionNormal * -1.0; }

    std::unordered_set<Primitive*> mySet = std::unordered_set<Primitive*>(insideObjects);
    if(enteringCurrentObject){ mySet.insert(currentObject);}
    else { mySet.erase(currentObject);}
    float ior_a = isInside ? IOR_GLASS : IOR_AIR;
    float ior_b = mySet.size() > 0 ? IOR_GLASS : IOR_AIR;



    // Adapted from http://steve.hollasch.net/cgindex/render/refraction.txt
    // Ni, Nt are incoming and transmitted index of refraction
    float eta = ior_a/ior_b;
    float c1 = -Vec3f::dot(direction, intersectionNormal);
    float cs2 = 1.0 - eta * eta * (1.0 - c1 * c1);
    if(cs2 < 0.0) {
        return Colr(0,1,0); // total internal reflection
    }
    Vec3f transmittedDirection = (direction * eta) + intersectionNormal * (eta * c1 - sqrt(cs2));
    Pos startPositionBumped = point + (intersectionNormal * BUMP_EPSILON * -1.0);

    Ray refractionRay = Ray(startPositionBumped, transmittedDirection);
    refractionRay.insideObjects = insideObjects;
    return refractionRay.trace(bounces);
}

Colr Ray::shadow(const LightIO* light) const {
    Colr shadowFactor = Colr(1,1,1);
        Vec3f lightVector;
        if(light->type == POINT_LIGHT){
            lightVector = Vec3f::normalize(Pos(light->position) - intersectionPoint());
        }
        else if(light->type == DIRECTIONAL_LIGHT){
            lightVector = Vec3f::normalize(Vec3f(light->direction)*-1.0);
        }
        Ray shadowRay = Ray(intersectionPoint(), lightVector);
        for( auto object : objects){
            shadowRay.t_max = INFINITY;
            if(object->intersect(shadowRay)){
                if(object->material.ktran < 0.001){ return Colr(0,0,0);}
                shadowFactor = shadowFactor * (Colr(object->material.diffColor).normalize())*object->material.ktran;
            }
        }
    return shadowFactor;
}

Colr Ray::diffuse(const LightIO* light) const {
    Colr result = Colr(0,0,0);

    Vec3f directionToLight;
    if(light->type == POINT_LIGHT){
        directionToLight = (Vec3f(light->position) - intersectionPoint()).normalize(); // Vector from the point to the light.
    } else if (light->type == DIRECTIONAL_LIGHT){
        directionToLight = (Vec3f(light->direction)*-1.0).normalize();
    }
    Colr currentLightContribution = Colr(material.diffColor) * fmax(0,Vec3f::dot(directionToLight,intersectionNormal)) * Colr(light->color) * attenuationFactor(intersectionPoint(), light);
    result = result + currentLightContribution;
    return result * (1-material.ktran);
}

Colr Ray::specular(const LightIO* light) const {
    Colr result = Colr(0,0,0);
    float q = material.shininess * 30.0;
    Colr Ks = Colr(material.specColor);
    Vec3f V = direction*(-1.0); // Incident flipped - ray from point to eye. Normalized.

    Vec3f L;
    if(light->type == POINT_LIGHT){
        L = (Vec3f(light->position) - intersectionPoint()).normalize(); // Vector from the point to the light.
    } else if (light->type == DIRECTIONAL_LIGHT){
        L = (Vec3f(light->direction)*-1.0).normalize();
    }
    Vec3f Q = intersectionNormal * Vec3f::dot(intersectionNormal, L);
    Vec3f R = ((Q * 2.0) - L).normalize();
    float dot = fmax(0.0,Vec3f::dot(R, V));
    float pow = powf(dot, q);
    result = result + Ks * pow * Colr(light->color) *  attenuationFactor(intersectionPoint(), light);
    return result;
}

float Ray::attenuationFactor(const Pos point, const LightIO* light) const {
    if(light->type == DIRECTIONAL_LIGHT){ return 1.0;}
    float c1 = 0.25;
    float c2 = 0.1;
    float c3 = 0.01;
    float d = (point - Vec3f(light->position)).length(); // distance between light and shaded point.
    return fmin(1.0, 1.0 / (c1 + c2*d + c3*d*d));
}

Colr Ray::ambient() const {
    return Colr(material.diffColor[0] * material.ambColor[0],
                material.diffColor[1] * material.ambColor[1],
                material.diffColor[2] * material.ambColor[2]) * (1.0-material.ktran);
}