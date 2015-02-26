//#include <windows.h>
#include <stdio.h>
#include <vector>
//#include <atlimage.h>
#include "scene_io.h"
#include "Timer.h"
#include "Vec3f.h"
#include "Sphere.h"
#include "Mesh.h"
#include "EasyBMP.h"
#include "Ray.h"
#include "kdTree.h"
#include "Framebuffer.h"
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define NUM_SAMPLES 4
#define MAX_BOUNCES	10

typedef unsigned char u08;

SceneIO *scene = NULL;
std::vector<LightIO*> lights;
std::vector<Primitive*> objects;

#pragma mark - Shaders
void mirror(Ray &ray, const bool on);
bool CHECKERBOARD(const float u, const float v);

void defaultShader(Ray &ray){
    if (ray.currentObject->name == NULL || ray.currentObject->name) {
        return;
    }
    else {
        int shader = atoi(ray.currentObject->name);
        switch (shader) {
            case 1:
                mirror(ray, CHECKERBOARD(ray.u, ray.v));
                break;
            default:
                break;
        }
    }
};


bool CHECKERBOARD(const float u, const float v){
    int CHECK_SIZE_X = 5;
    int CHECK_SIZE_Y = 5;
    float xbase = 1.0 / CHECK_SIZE_X;
    float ybase = 1.0 / CHECK_SIZE_Y;
    float x = fmod(u, 2.0/CHECK_SIZE_X);
    float y = fmod(v, 2.0/CHECK_SIZE_Y);
    return ((x > xbase && y < ybase )|| (x < xbase  && y > ybase));
}



void mirror(Ray &ray, bool on){
    if (!on){ return; }
    ray.material.specColor[0] = 1;
    ray.material.specColor[1] = 1;
    ray.material.specColor[2] = 1;

    ray.material.diffColor[0] = 0;
    ray.material.diffColor[1] = 0;
    ray.material.diffColor[2] = 0;

    ray.material.ambColor[0] = 0;
    ray.material.ambColor[1] = 0;
    ray.material.ambColor[2] = 0;

    ray.material.emissColor[0] = 0;
    ray.material.emissColor[1] = 0;
    ray.material.emissColor[2] = 0;

    ray.material.shininess = 3;
    ray.material.ktran = 0;
}


static void loadScene(char *name) {
    std::cout << "Loading scene" << name <<std::endl;
	/* load the scene into the SceneIO data structure using given parsing code */
	scene = readScene(name);

	/* hint: use the Visual Studio debugger ("watch" feature) to probe the
	   scene data structure and learn more about it for each of the given scenes */

    /* write any code to transfer from the scene data structure to your own here */
    /* */
    ObjIO * nextObj = scene->objects;
    while (nextObj != nullptr){
        if (nextObj->type == SPHERE_OBJ) {
            SphereIO* sphere = (SphereIO*)nextObj->data;
            MaterialIO* material = nextObj->material;
            objects.push_back(new Sphere(*sphere, *material, nextObj->name));
        }
        if( nextObj->type == POLYSET_OBJ){
            PolySetIO* polyset = (PolySetIO*)nextObj->data;
            MaterialIO* material = nextObj->material;
            Mesh* mesh = new Mesh(*polyset, material, nextObj->numMaterials);
            objects.push_back(mesh);
        }
        nextObj = nextObj->next;
    }
    LightIO *nextLight = scene->lights;
    while (nextLight != nullptr) {
        lights.push_back(nextLight);
        nextLight = nextLight->next;
    }
    std::cout << "Done loading" << std::endl;
	return;
}

void cleanupScene(){
    if (scene != NULL) {
        deleteScene(scene);
    }
    objects.clear();
    lights.clear();

}


/* just a place holder, feel free to edit */
void render(char* filename) {
    Framebuffer buf = Framebuffer(IMAGE_WIDTH, IMAGE_HEIGHT, NUM_SAMPLES);
    std::cout << "Rendering " << filename<< std::endl;
    buf.render(filename);


    std::cout << "Done rendering." << std::endl;
}

int main(int argc, char *argv[]) {
    Timer total_timer;
    total_timer.start();


#pragma mark - Scene 1

    Timer scene1_build_timer, scene1_draw_timer, scene1_total_timer;
    scene1_build_timer.start();
    scene1_draw_timer.start();
    scene1_total_timer.start();
    loadScene("../Scenes2/test1.ascii");
    scene1_build_timer.stop();
    render("test1.bmp");
    scene1_draw_timer.stop();
    cleanupScene();
    scene1_total_timer.stop();

#pragma mark Scene 2

    Timer scene2_build_timer, scene2_draw_timer, scene2_total_timer;
    scene2_build_timer.start();
    scene2_draw_timer.start();
    scene2_total_timer.start();
    loadScene("../Scenes2/test2.ascii");
    scene2_build_timer.stop();
    render("test2.bmp");
    scene2_draw_timer.stop();
    cleanupScene();
    scene2_total_timer.stop();

# pragma mark Scene 3

    Timer scene3_build_timer, scene3_draw_timer, scene3_total_timer;
    scene3_build_timer.start();
    scene3_draw_timer.start();
    scene3_total_timer.start();
    loadScene("../Scenes2/test3.ascii");
    scene3_build_timer.stop();
    render("test3.bmp");
    scene3_draw_timer.stop();
    cleanupScene();
    scene3_total_timer.stop();

#pragma mark - Scene 4

    Timer scene4_build_timer, scene4_draw_timer, scene4_total_timer;
    scene4_build_timer.start();
    scene4_draw_timer.start();
    scene4_total_timer.start();
    loadScene("../Scenes2/test4.ascii");
    scene4_build_timer.stop();
    render("test4.bmp");
    scene4_draw_timer.stop();
    cleanupScene();
    scene4_total_timer.stop();


#pragma mark - Scene 5

    Timer scene5_build_timer, scene5_draw_timer, scene5_total_timer;
    scene5_build_timer.start();
    scene5_draw_timer.start();
    scene5_total_timer.start();
    loadScene("../Scenes2/test5.ascii");
    scene5_build_timer.stop();
    render("test5.bmp");
    scene5_draw_timer.stop();
    cleanupScene();
    scene5_total_timer.stop();

    std::cout << "Scene1. Load: " << scene1_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << scene1_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << scene1_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

    std::cout << "Scene2. Load: " << scene2_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << scene2_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << scene2_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

    std::cout << "Scene3. Load: " << scene3_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << scene3_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << scene3_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

    std::cout << "Scene4. Load: " << scene4_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << scene4_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << scene4_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

    std::cout << "Scene5. Load: " << scene5_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << scene5_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << scene5_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

    total_timer.stop();
    std::cout << "Total time for all scenes: " << total_timer.getElapsedTimeInMilliSec() << "ms." << std::endl;
    std::cout << "Resolution was " << IMAGE_HEIGHT << "*" << IMAGE_WIDTH << ", with " << NUM_SAMPLES << "per pixel." << std::endl;
    return 1;}
