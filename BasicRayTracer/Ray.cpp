#include "Ray.h"
#include "Sphere.h"
extern SceneIO *scene;
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

    ObjIO * nextObj = scene->objects;
    while (nextObj != nullptr){
        // Sphere
        if (nextObj->type == SPHERE_OBJ) {
            SphereIO* sphere = (SphereIO*)nextObj->data;
            MaterialIO* material = nextObj->material;
            Sphere sp = Sphere(*sphere, *material);
            sp.intersect(*this);
        }
        // Mesh
        else if(0){
            // ...
        }
        nextObj = nextObj->next;
    }
    return color;
}