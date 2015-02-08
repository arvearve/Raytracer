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

/* Placeholder. Expand to handle transparent objects -> colored shadow.
   Perhaps merge with trace?
 */
//bool Ray::shadowTrace(){
//
//}



Colr Ray::trace(int bounces){

    // Find which object we intersect closest:
    for ( Primitive* object : objects ) {
        object->intersect(*this);
    }
    if(t_max == INFINITY){ // No hit.
        return BACKGROUND_COLOR;
    }
    Colr diffuseColor = diffuse(intersectionPoint());
    Colr ambientColor = ambient();

    // Shadow ray

    // Reflection ray
    return ambientColor + diffuseColor;
}

Colr Ray::diffuse(const Pos point) const {
    Colr result = Colr(0,0,0);
    for(LightIO* light : lights){
        Vec3f directionToLight = (Pos(light->position) - point).normalize();
        result = result + Colr(material.diffColor) * fmax(0,Vec3f::dot(directionToLight,intersectionNormal));
        result = result * (1.0/sqrt(3));
    }
    return result * (1-material.ktran);
}

Colr Ray::ambient() const {
    return Colr(material.diffColor[0] * material.ambColor[0],
                material.diffColor[1] * material.ambColor[1],
                material.diffColor[2] * material.ambColor[2]) * (1.0-material.ktran);
}