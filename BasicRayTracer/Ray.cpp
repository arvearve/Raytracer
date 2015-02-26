#include "Ray.h"
#include "Sphere.h"
extern void defaultShader(Ray &ray);
extern SceneIO *scene;
extern std::vector<Primitive*> objects;
extern std::vector<LightIO*> lights;
size_t Ray::counter = 0;
Ray::Ray(Pos startPosition, Vec3f direction)
:_id(++counter), startPosition(startPosition),
direction(direction.normalize()),
inv_direction(Vec3f(1.0/direction.x, 1.0/direction.y, 1.0/direction.z))
{
    sign[0] = (inv_direction.x < 0);
    sign[1] = (inv_direction.y < 0);
    sign[2] = (inv_direction.z < 0);
    surfaceShader = defaultShader;
    t_max = INFINITY;
    color = BACKGROUND_COLOR;
}

Pos Ray::intersectionPoint() const {
    return startPosition + direction * t_max;
}

Colr Ray::trace(int bounces){
    return trace(bounces, std::unordered_set<Primitive*>());
}

Colr Ray::trace(int bounces, std::unordered_set<Primitive*> insideObjects){
    if(bounces <= 0){return Colr(0,0,0);}
    // Find which object we intersect closest:
    for ( Primitive* object : objects ) {
        object->intersect(*this);
    }
    if(t_max == INFINITY){ // No hit.
        return BACKGROUND_COLOR;
    }

    // We hit something, and have aquired it's material. Run surface shader
    defaultShader(*this);

    // Keep track of which objects we have crossed into, for refraction rays etc..
    // Are we currently inside an object?
    bool isInside = insideObjects.size() > 0;
    // currentObject is a pointer to the closest object we are intersecting.
    // If the set contains it, we are already inside it.
    bool enteringCurrentObject = insideObjects.find(currentObject) == insideObjects.end();

    if(!enteringCurrentObject){
        intersectionNormal = intersectionNormal * -1.0;
    }

    /* Figure out the color to return: */

    Colr ambientColor = ambient();

    /* Per light stuff: diffuse, specular, shadow: */
    Colr diffuseColor, specularColor;
    float attenuation;
    for (LightIO* light: lights){
        Colr color = Colr(light->color);
        Vec3f lightDirection;
        float lightDistance;
        if(light->type == POINT_LIGHT){
            lightDirection = (Vec3f(light->position) - intersectionPoint()); // Vector from the point to the light.
            lightDistance = lightDirection.length();
            lightDirection.normalize();

        } else if (light->type == DIRECTIONAL_LIGHT){
            lightDirection = (Vec3f(light->direction)*-1.0).normalize();
            lightDistance = INFINITY;
        }else{
            std::cout << "Error: Unsupported light type!" << std::endl;
            return Colr(1,0,1);
        }
        attenuation = attenuationFactor(intersectionPoint(), light);
        Vec3f shadowFactor = shadow(lightDirection, lightDistance);
        diffuseColor += diffuse(lightDirection, color) * shadowFactor * attenuation;
        specularColor += specular(lightDirection, color) * shadowFactor * attenuation;
    }

    Colr reflectionColor = Colr(0,0,0);
    if(isReflective()) {
        reflectionColor = reflection(intersectionPoint(), bounces-1, insideObjects);
    }

    Colr refractionColor = Colr(0,0,0);
    if(isTransparent()){
        // If we are entering a new object, add it to the set.
        std::unordered_set<Primitive*> mySet(insideObjects);
        if(enteringCurrentObject){
            mySet.insert(currentObject);
        }
        else {
            mySet.erase(currentObject);
        }
        float ior_a = isInside ? IOR_GLASS : IOR_AIR;
        float ior_b = mySet.size() != 0 ? IOR_GLASS : IOR_AIR;
        refractionColor = refraction(intersectionPoint(), bounces-1, ior_a, ior_b, mySet, insideObjects);
    }

    Colr result = ambientColor + diffuseColor + specularColor + reflectionColor + refractionColor;
    result.capColor();
    return result;
}

bool Ray::isReflective() const {
    return (material.specColor[0] > 0.0
            || material.specColor[1] > 0.0
            || material.specColor[2] > 0.0) && t_max < INFINITY;
}

bool Ray::isTransparent() const {
    return material.ktran >= 0.001 && t_max < INFINITY;
}

Colr Ray::reflection(const Pos point, const int bounces, const std::unordered_set<Primitive*> mySet) const {
    Vec3f incident = Vec3f::normalize(direction);
    double cosI = -Vec3f::dot(intersectionNormal, incident);
    Vec3f reflectedDirection =  incident + intersectionNormal * cosI * 2;
    Ray reflectionRay = Ray(point + intersectionNormal*BUMP_EPSILON, reflectedDirection);
    Colr reflectionColor = reflectionRay.trace(bounces, mySet);
    return reflectionColor * Colr(material.specColor);

}

Colr Ray::refraction(const Pos point, const int bounces, const float ior_a, const float ior_b, const std::unordered_set<Primitive*> mySet, const std::unordered_set<Primitive*> oldSet){
    Vec3f incident = direction * -1.0;
    float n = ior_b / ior_a;
    float cosThetaI = Vec3f::dot(incident, intersectionNormal);
    float thetaI = acos(cosThetaI);
    float sinThetaT = (ior_a/ior_b) * sin(thetaI);
    float thetaT = asin(sinThetaT);
    float cosThetaT = cos(thetaT);

    Vec3f newDirection = incident * -(1.0/n) - intersectionNormal * (cosThetaT - (1.0/n) * cosThetaI);

    if (thetaI >= asin(ior_b/ior_a)) {
        return Ray(intersectionPoint()+intersectionNormal*BUMP_EPSILON, newDirection).trace(bounces-1, oldSet);
    }
    else{
        return Ray(intersectionPoint() - intersectionNormal * BUMP_EPSILON, newDirection).trace(bounces-1, mySet) * material.ktran;
    }

}


Colr Ray::shadow(const Vec3f &L, const float lightDistance) const {
    Colr shadowFactor = Colr(1,1,1);
    for( auto object : objects){
        Ray shadowRay = Ray(intersectionPoint() + intersectionNormal*BUMP_EPSILON , L);
        if(object->intersect(shadowRay)){
            Vec3f intersectVector = shadowRay.intersectionPoint() - shadowRay.startPosition;
            if( (intersectVector.length() >= lightDistance) ){ continue; }
            if(shadowRay.material.ktran < 0.001f){return Colr(0,0,0);}
            shadowFactor = shadowFactor * (Colr(shadowRay.material.diffColor).normalizeColor()) * shadowRay.material.ktran;
        }
    }
    return shadowFactor;
}

Colr Ray::diffuse(const Vec3f &L, Colr &lightColor) const {
    Colr result = Colr(material.diffColor) * fmax(0,Vec3f::dot(L,intersectionNormal)) * lightColor;
    return result * (1.0-material.ktran);
}

Colr Ray::specular(const Vec3f &L, Colr &color) const {
    float q = material.shininess * 30.0;
    Colr Ks = Colr(material.specColor);
    Vec3f V = direction*(-1.0); // Incident flipped - ray from point to eye. Normalized.
    Vec3f Q = intersectionNormal * Vec3f::dot(intersectionNormal, L);
    Vec3f R = ((Q * 2.0) - L).normalize();
    float dot = fmax(0.0,Vec3f::dot(R, V));
    float pow = powf(dot, q);
    return Ks * pow * color;

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