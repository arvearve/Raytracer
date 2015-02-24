//#include <windows.h>
#include <stdio.h>
#include <vector>
//#include <atlimage.h>
#include "scene_io.h"
//#include "Timer.h"
#include "Vec3f.h"
#include "Sphere.h"
#include "Mesh.h"
#include "EasyBMP.h"
#include "Ray.h"
#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512
#define MAX_BOUNCES	10

typedef unsigned char u08;

SceneIO *scene = NULL;
std::vector<LightIO*> lights;
std::vector<Primitive*> objects;

#pragma mark - Shaders
void mirror(Ray &ray, const bool on);
bool CHECKERBOARD(const float u, const float v);

void defaultShader(Ray &ray){
    if (ray.currentObject->name == NULL) {
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
            objects.push_back(new Mesh(*polyset, material, nextObj->numMaterials));
        }
        nextObj = nextObj->next;
    }
    LightIO *nextLight = scene->lights;
    while (nextLight != nullptr) {
        lights.push_back(nextLight);
        nextLight = nextLight->next;
    }

	return;
}


/* just a place holder, feel free to edit */
void render(void) {
    Pos E = Pos(scene->camera->position); // Eye position
    Vec3f V = Vec3f(scene->camera->viewDirection).normalize(); // View direction
    Vec3f U = Vec3f(scene->camera->orthoUp).normalize(); // Camera Up vector (orthoUp)
    float fovVertical = scene->camera->verticalFOV;

    int w = IMAGE_WIDTH;
    int h = IMAGE_HEIGHT;
    float fovHorizontal = fovVertical * ((float)w/(float)h);
    Vec3f A = Vec3f::cross(V, U); // Right vector
    Vec3f B = Vec3f::cross(A, V); // Up vector aligned with image plane.
    A = A.normalize();
    B = B.normalize();
    float c = 1;
    Pos M = E + (V*c); // Middle of image plane.
    Vec3f Y = B * (c * tan(fovVertical / 2.0));
    Vec3f X = A * (c * tan(fovHorizontal / 2.0));



//	CImage image;
//	image.Create(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    BMP image;
    image.SetBitDepth(24);
    image.SetSize(IMAGE_WIDTH, IMAGE_HEIGHT);
	for (int j = 0; j < IMAGE_HEIGHT; j++) {
        std::cout  << "Rendering: " << j << "/" << IMAGE_HEIGHT << std::endl;
		for (int i = 0; i < IMAGE_WIDTH; i++) {
			float sx = (i + 1.0 / 2.0) * (1.0 / IMAGE_WIDTH);
			float sy = (j + 1.0 / 2.0) * (1.0 / IMAGE_HEIGHT);
			Pos P = M + X*(2.0 * sx - 1.0) + Y * (2.0 * sy - 1.0);
            Ray ray = Ray(E, P-E);
            Colr rayColor = ray.trace(10);

            RGBApixel *pixel = image(i, IMAGE_HEIGHT - j - 1);
            pixel->Red = rayColor.x*255;
            pixel->Green = rayColor.y*255;
            pixel->Blue = rayColor.z*255;

		}
	}

	/* save out the image */
//	image.Save(_T("raytraced.png"), Gdiplus::ImageFormatPNG);
    image.WriteToFile("raytraced2.bmp");
    /* cleanup */

	return;
}



int main(int argc, char *argv[]) {
//	Timer total_timer;
//	total_timer.startTimer();

	loadScene((char *)"../Scenes2/test2.ascii");

	/* write your ray tracer here */
	render();

	/* cleanup */
	if (scene != NULL) {
		deleteScene(scene);
	}

//	total_timer.stopTimer();
//	fprintf(stderr, "Total time: %.5lf secs\n\n", total_timer.getTime());

	return 0;
}
