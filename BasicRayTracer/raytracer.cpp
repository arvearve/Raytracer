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
#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512
#define NUM_SAMPLES 64
#define SENSOR_DISTANCE 1

typedef unsigned char u08;

SceneIO *scene = NULL;
std::vector<LightIO*> lights;
std::vector<Primitive*> objects;
std::vector<Mesh*> areaLights;

#pragma mark - Shaders
void mirror(Ray &ray, const bool on);
void earth(Ray &ray, const bool on);
bool CHECKERBOARD(const float u, const float v);
void defaultShader(Ray &ray){
    if (ray.currentObject->name == NULL) {
        return;
    }
    else {
        int shader = atoi(ray.currentObject->name);
        switch (shader%10) {
            case 1:
                earth(ray, true);
                mirror(ray, CHECKERBOARD( ray.u/2, ray.v/2));
                break;
            case 2:
                earth(ray, true);
            case 3:
                ray.material.diffColor[0] = CHECKERBOARD(ray.u, ray.v);
                ray.material.diffColor[1] = CHECKERBOARD(ray.u, ray.v);
                ray.material.diffColor[2] = CHECKERBOARD(ray.u, ray.v);
            default:
                break;
        }
    }
};



bool CHECKERBOARD(const float u, const float v){
    int CHECK_SIZE_X = 10;
    int CHECK_SIZE_Y = 10;
    float xbase = 1.0 / CHECK_SIZE_X;
    float ybase = 1.0 / CHECK_SIZE_Y;
    float x = fmod(u, 2.0/CHECK_SIZE_X);
    float y = fmod(v, 2.0/CHECK_SIZE_Y);
    return ((x > xbase && y < ybase )|| (x < xbase  && y > ybase));
}


void earth(Ray& ray, bool on){
    if(!on) { return; }
    ray.material.diffColor[0] = 0.3;
    ray.material.diffColor[1] = 0.3;
    ray.material.diffColor[2] = 1;
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
            Mesh* mesh = new Mesh(*polyset, material, nextObj->numMaterials, nextObj->name);
            objects.push_back(mesh);
            if(mesh->materials[0].emissColor[0] > 0){
                areaLights.push_back(mesh);
            }
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
void render(char* filename, int numSamples) {
    Framebuffer buf = Framebuffer(IMAGE_WIDTH, IMAGE_HEIGHT, numSamples);
    std::cout << "Rendering " << filename<< std::endl;
//    buf.renderLens(filename, SENSOR_DISTANCE);
    buf.renderPinhole(filename, SENSOR_DISTANCE);
    std::cout << "Done rendering." << std::endl;
}

int main(int argc, char *argv[]) {
    Timer total_timer;
    total_timer.start();


#pragma mark - Fun scene

    Timer fun_scene_build_timer, fun_scene_draw_timer, fun_scene_total_timer;
    fun_scene_total_timer.start();

    fun_scene_build_timer.start();
    loadScene("../Scenes2/cornell_arealight.ascii");
    fun_scene_build_timer.stop();

    fun_scene_draw_timer.start();
    render("cornell.bmp", NUM_SAMPLES);
    fun_scene_draw_timer.stop();

    cleanupScene();
    fun_scene_total_timer.stop();
//
//#pragma mark - Scene 1
//
//    Timer scene1_build_timer, scene1_draw_timer, scene1_total_timer;
//    scene1_total_timer.start();
//
//    scene1_build_timer.start();
//    loadScene("../Scenes2/test1.ascii");
//    scene1_build_timer.stop();
//
//    scene1_draw_timer.start();
//    render("hw3_test1.bmp",1);
//    scene1_draw_timer.stop();
//
//    cleanupScene();
//    scene1_total_timer.stop();
//
//#pragma mark Scene 2
//
//    Timer scene2_build_timer, scene2_draw_timer, scene2_total_timer;
//    scene2_total_timer.start();
//
//    scene2_build_timer.start();
//    loadScene("../Scenes2/test2.ascii");
//    scene2_build_timer.stop();
//
//    scene2_draw_timer.start();
//    render("hw3_test2.bmp", 1);
//    scene2_draw_timer.stop();
//
//    cleanupScene();
//    scene2_total_timer.stop();
//
//
//# pragma mark Scene 3
//
//    Timer scene3_build_timer, scene3_draw_timer, scene3_total_timer;
//    scene3_total_timer.start();
//
//    scene3_build_timer.start();
//    loadScene("../Scenes2/test3.ascii");
//    scene3_build_timer.stop();
//
//    scene3_draw_timer.start();
//    render("hw3_test3.bmp", 1);
//    scene3_draw_timer.stop();
//
//    cleanupScene();
//    scene3_total_timer.stop();
//
//#pragma mark - Scene 4
//
//    Timer scene4_build_timer, scene4_draw_timer, scene4_total_timer;
//    scene4_total_timer.start();
//
//    scene4_build_timer.start();
//    loadScene("../Scenes2/test4.ascii");
//    scene4_build_timer.stop();
//
//    scene4_draw_timer.start();
//    render("hw3_test4.bmp", 1);
//    scene4_draw_timer.stop();
//
//    cleanupScene();
//    scene4_total_timer.stop();
//
//
//#pragma mark - Scene 5
//
//    Timer scene5_build_timer, scene5_draw_timer, scene5_total_timer;
//    scene5_total_timer.start();
//
//    scene5_build_timer.start();
//    loadScene("../Scenes2/test5.ascii");
//    scene5_build_timer.stop();
//
//    scene5_draw_timer.start();
//    render("hw3_test5.bmp", 1);
//    scene5_draw_timer.stop();
//
//    cleanupScene();
//    scene5_total_timer.stop();

    std::cout << "Fun scene. Load: " << fun_scene_build_timer.getElapsedTimeInMilliSec()
    << "ms, Draw: "  << fun_scene_draw_timer.getElapsedTimeInMilliSec()
    << "ms, Total: " << fun_scene_total_timer.getElapsedTimeInMilliSec()
    << "ms." << std::endl;

//    std::cout << "Scene1. Load: " << scene1_build_timer.getElapsedTimeInMilliSec()
//    << "ms, Draw: "  << scene1_draw_timer.getElapsedTimeInMilliSec()
//    << "ms, Total: " << scene1_total_timer.getElapsedTimeInMilliSec()
//    << "ms." << std::endl;
//
//    std::cout << "Scene2. Load: " << scene2_build_timer.getElapsedTimeInMilliSec()
//    << "ms, Draw: "  << scene2_draw_timer.getElapsedTimeInMilliSec()
//    << "ms, Total: " << scene2_total_timer.getElapsedTimeInMilliSec()
//    << "ms." << std::endl;
//
//    std::cout << "Scene3. Load: " << scene3_build_timer.getElapsedTimeInMilliSec()
//    << "ms, Draw: "  << scene3_draw_timer.getElapsedTimeInMilliSec()
//    << "ms, Total: " << scene3_total_timer.getElapsedTimeInMilliSec()
//    << "ms." << std::endl;
//
//    std::cout << "Scene4. Load: " << scene4_build_timer.getElapsedTimeInMilliSec()
//    << "ms, Draw: "  << scene4_draw_timer.getElapsedTimeInMilliSec()
//    << "ms, Total: " << scene4_total_timer.getElapsedTimeInMilliSec()
//    << "ms." << std::endl;
//
//    std::cout << "Scene5. Load: " << scene5_build_timer.getElapsedTimeInMilliSec()
//    << "ms, Draw: "  << scene5_draw_timer.getElapsedTimeInMilliSec()
//    << "ms, Total: " << scene5_total_timer.getElapsedTimeInMilliSec()
//    << "ms." << std::endl;


    total_timer.stop();
    std::cout << "Total time for all scenes: " << total_timer.getElapsedTimeInMilliSec() << "ms." << std::endl;
    std::cout << "Resolution was " << IMAGE_HEIGHT << "*" << IMAGE_WIDTH << ", with " << NUM_SAMPLES << " samples per pixel." << std::endl;
    return 1;
}
