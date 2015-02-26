//
//  Framebuffer.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 26/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Framebuffer.h"

float Framebuffer::jitter(const float distance) const{
    return -distance + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(distance*2)));
}

extern SceneIO *scene;

void Framebuffer::render(char* filename) {

    Pos E = Pos(scene->camera->position); // Eye position
    for (Pixel &pixel : pixels) {
        for (Sample &sample: pixel.samples){
            Ray ray = Ray(E, sample.pos-E);
            sample.color = ray.trace(10);
        }
        pixel.filter();
    }
    saveFile(filename);
    std::cout << "Done." << std::endl;
    return;
}

void Pixel::filter(){
    for (Sample sample:samples){
        filteredColor.x += sample.color.x;
        filteredColor.y += sample.color.y;
        filteredColor.z += sample.color.z;
    }
    filteredColor.x = filteredColor.x / samples.size();
    filteredColor.y = filteredColor.y / samples.size();
    filteredColor.z = filteredColor.z / samples.size();
}


void Framebuffer::saveFile(char *filename){

    //	CImage image;
    //	image.Create(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    BMP image;
    image.SetBitDepth(24);
    image.SetSize(WIDTH, HEIGHT);
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w<WIDTH; w++) {
            int currentIndex = h*WIDTH+w;
            RGBApixel *pixel = image(w, HEIGHT - h - 1);
            Colr c = pixels[currentIndex].filteredColor;
            pixel->Red = c.x*255;
            pixel->Green = c.y*255;
            pixel->Blue = c.z*255;

        }
    }
    /* save out the image */
    //	image.Save(_T("raytraced.png"), Gdiplus::ImageFormatPNG);
    image.WriteToFile(filename);
    /* cleanup */
}

void Framebuffer::init(){
    pixels = std::vector<Pixel>();

    Pos E = Pos(scene->camera->position); // Eye position
    Vec3f V = Vec3f(scene->camera->viewDirection).normalize(); // View direction
    Vec3f U = Vec3f(scene->camera->orthoUp).normalize(); // Camera Up vector (orthoUp)
    float fovVertical = scene->camera->verticalFOV;

    int w = WIDTH;
    int h = HEIGHT;
    float fovHorizontal = fovVertical * ((float)w/(float)h);
    Vec3f A = Vec3f::cross(V, U); // Right vector
    Vec3f B = Vec3f::cross(A, V); // Up vector aligned with image plane.
    A = A.normalize();
    B = B.normalize();
    float c = 1;
    Pos M = E + (V*c); // Middle of image plane.
    Vec3f Y = B * (c * tan(fovVertical / 2.0));
    Vec3f X = A * (c * tan(fovHorizontal / 2.0));

    float dw = 1.0/WIDTH;
    float dh = 1.0/HEIGHT;
    float sampleOffset = 1.0/samples;
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            float sx = (i + 0.5) * dw;
            float sy = (j + 0.5) * dh;
            Pos PixelBottomLeftPosition = M + X*(2.0 * sx - 1.0) + Y * (2.0 * sy - 1.0);
            Pixel p = Pixel(samples);
            for(int sampleCountY = 0; sampleCountY < samples; sampleCountY++){
                for(int sampleCountX = 0; sampleCountX < samples; sampleCountX++){
                    Pos samplePosition = PixelBottomLeftPosition
                        + X * sampleOffset * sampleCountX * jitter(sampleOffset/2.0)
                        + Y * sampleOffset * sampleCountY * jitter(sampleOffset/2.0) ;
                    Sample sample = Sample(samplePosition);
                    p.samples.push_back(sample);
                }
            }
            pixels.push_back(p);
        }
    }
}