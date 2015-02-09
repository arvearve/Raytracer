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

Pos Ray::intersectionPoint(){
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
    Colr diffuseColor = diffuse(intersectionPoint());
    Colr ambientColor = ambient();
    Colr specularColor = specular(intersectionPoint());
    Colr reflectionColor;
    if(material.specColor[0] == 0.0
       && material.specColor[1] == 0.0
       && material.specColor[2] == 0.0){
        reflectionColor = Colr(0,0,0);
    }
    else {
        reflectionColor = reflection(intersectionPoint(), bounces-1) * Colr(material.specColor);
    }
    Colr result = ambientColor + diffuseColor + specularColor + reflectionColor;
    result.capColor();
    return result;
}

Colr Ray::reflection(const Pos point, const int bounces) const {
    Vec3f incident = direction * -1.0;
    Vec3f R = intersectionNormal* 2.0 * Vec3f::dot(incident, intersectionNormal)  - incident;
    Ray reflectionRay = Ray(point, R);
    Colr reflectionColor = reflectionRay.trace(bounces);
    return reflectionColor;

}

Colr Ray::shadow(const Pos point, const LightIO light) const {
    Colr shadowFactor = Colr(1,1,1);
    Vec3f lightVector;
    if(light.type == POINT_LIGHT){
        lightVector = Vec3f::normalize(Pos(light.position) - point);
    }
    else if(light.type == DIRECTIONAL_LIGHT){
        lightVector = Vec3f::normalize(Vec3f(light.direction)*-1.0);
    }

    Ray shadowRay = Ray(point, lightVector);
    for( auto object : objects){
        shadowRay.t_max = INFINITY;
        if(object->intersect(shadowRay)){
            shadowFactor = Colr(0,0,0);
        }
    }
    return shadowFactor;
}

Colr Ray::diffuse(const Pos point) const {
    Colr result = Colr(0,0,0);
    for(LightIO* light : lights){

        Vec3f directionToLight;
        if(light->type == POINT_LIGHT){
            directionToLight = (Vec3f(light->position) - point).normalize(); // Vector from the point to the light.
        } else if (light->type == DIRECTIONAL_LIGHT){
            directionToLight = (Vec3f(light->direction)*-1.0).normalize();
        }
        Colr currentLightContribution = Colr(material.diffColor) * fmax(0,Vec3f::dot(directionToLight,intersectionNormal)) * shadow(point, *light) * Colr(light->color) * attenuationFactor(point, light);
        result = result + currentLightContribution;
    }
    return result * (1-material.ktran);
}

Colr Ray::specular(const Pos point) const {
    Colr result = Colr(0,0,0);
    float q = material.shininess * 30.0;
    Colr Ks = Colr(material.specColor);
    Vec3f V = direction*(-1.0); // Incident flipped - ray from point to eye. Normalized.
    for( auto light:lights) {
        Vec3f L;
        if(light->type == POINT_LIGHT){
            L = (Vec3f(light->position) - point).normalize(); // Vector from the point to the light.
        } else if (light->type == DIRECTIONAL_LIGHT){
            L = (Vec3f(light->direction)*-1.0).normalize();
        }
        Vec3f Q = intersectionNormal * Vec3f::dot(intersectionNormal, L);
        Vec3f R = ((Q * 2.0) - L).normalize();
        float dot = fmax(0.0,Vec3f::dot(R, V));
        float pow = powf(dot, q);
        result = result + Ks * pow * Colr(light->color) * shadow(point, *light) * attenuationFactor(point, light);
    }
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