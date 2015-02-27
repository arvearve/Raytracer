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


// Filter the samples contained in each pixel.
// This is where we would hook up an image shader for HDR stuff and so on.
void Framebuffer::filter(){
    for (Pixel &pixel : pixels) {
        int count = pixel.samples.size();
        for (Colr sample:pixel.samples){
            pixel.filteredColor.x += sample.x;
            pixel.filteredColor.y += sample.y;
            pixel.filteredColor.z += sample.z;
        }
        pixel.filteredColor.x = pixel.filteredColor.x / count;
        pixel.filteredColor.y = pixel.filteredColor.y / count;
        pixel.filteredColor.z = pixel.filteredColor.z / count;
    }
}


void Framebuffer::saveFile(char *filename, bool flip){

    //	CImage image;
    //	image.Create(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    BMP image;
    image.SetBitDepth(24);
    image.SetSize(WIDTH, HEIGHT);
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w<WIDTH; w++) {
            int currentIndex;
            if(flip){currentIndex = HEIGHT*WIDTH - (h*WIDTH+w) -2;}
            else { currentIndex = h*WIDTH+w; }
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

void Framebuffer::renderLens(char* filename, const float sensorDistance){
    auto result = std::vector<Pixel>();
    Pos E = Pos(scene->camera->position); // Eye position
    Vec3f V = Vec3f(scene->camera->viewDirection).normalize(); // View direction
    Vec3f U = Vec3f(scene->camera->orthoUp).normalize(); // Camera Up vector (orthoUp)
    int w = WIDTH;
    int h = HEIGHT;
    float fovVertical = scene->camera->verticalFOV;
    float fovHorizontal = fovVertical * ((float)w/(float)h);

    //Lens Plane:
    Pos LensCenter = E;
    Vec3f LensX = Vec3f::cross(V, U).normalize();
    Vec3f LensY = Vec3f::cross(LensX, V).normalize();

    // Sensor plane:
    Pos SensorCenter = LensCenter - V * sensorDistance;
    Vec3f SensorX = LensX * sensorDistance * tanf(fovHorizontal / 2.0);
    Vec3f SensorY = LensY * sensorDistance * tanf(fovVertical / 2.0);

    // Focal plane
    Pos FocalPlaneCenter = LensCenter + V * scene->camera->focalDistance ;
    Vec3f FocalPlaneNormal = V*-1.0;

    // Create framebuffer pixels with their samples
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            // Point on image plane
            float deltaSensorX = (i + 0.5) / WIDTH;
            float deltaSensorY = (j + 0.5) / HEIGHT;
            // Pixel position on image sensor
            Pos pixelPosition = SensorCenter + SensorX*(2.0 * deltaSensorX - 1.0) + SensorY * (2.0 * deltaSensorY -1.0);

            Pixel currentPixel = Pixel(samples, Pos());
            // Calculate sample positions
            // This is a position on the lens
            // Lens has radius 1.

            float sampleOffsetX = 1.0/(samples*WIDTH);
            float sampleOffsetY = 1.0/(samples*HEIGHT);
            for(int sampleCountY = 0; sampleCountY < samples; sampleCountY++){
                for(int sampleCountX = 0; sampleCountX < samples; sampleCountX++){

                    // Origin position on sensor
                    Pos samplePositionOnSensor = pixelPosition
                    + SensorX * sampleOffsetX * sampleCountX
                    + SensorY * sampleOffsetY * sampleCountY;

                    Vec3f directionFromSamplePositionToFocusPoint = (samplePositionOnSensor - LensCenter);

                    // Corresponding focus point
                    float r1 = Vec3f::dot(FocalPlaneNormal, FocalPlaneCenter - samplePositionOnSensor);
                    float r2 = Vec3f::dot(FocalPlaneNormal, directionFromSamplePositionToFocusPoint);
                    float r = r1/r2;
                    Vec3f focusPoint = samplePositionOnSensor + directionFromSamplePositionToFocusPoint * r;

                    // Randomize lens plane point
                    Pos lensPosition = LensCenter + LensX * jitter(0.8) // random along x-axis [-1,1]
                                                  + LensY * jitter(0.8);
                    Vec3f rayDirection = (focusPoint - lensPosition);
                    Colr sample  = Ray(lensPosition, rayDirection).trace(10);
                    currentPixel.samples.push_back(sample);
                }
            }
            pixels.push_back(currentPixel);
        }
    }

    // Filter samples
    filter();
    saveFile(filename, true);
}

void Framebuffer::renderPinhole(char* filename, const float sensorDistance){
    Pos E = Pos(scene->camera->position); // Eye position
    Vec3f V = Vec3f(scene->camera->viewDirection).normalize(); // View direction
    Vec3f U = Vec3f(scene->camera->orthoUp).normalize(); // Camera Up vector (orthoUp)


    int w = WIDTH;
    int h = HEIGHT;
    float fovVertical = scene->camera->verticalFOV;
    float fovHorizontal = fovVertical * ((float)w/(float)h);

    Vec3f A = Vec3f::cross(V, U); // Right vector
    Vec3f B = Vec3f::cross(A, V); // Up vector aligned with image plane.
    A = A.normalize();
    B = B.normalize();
    float c = sensorDistance;
    Pos M = E + (V*c); // Middle of image plane.
    Vec3f Y = B * (c * tan(fovVertical / 2.0));
    Vec3f X = A * (c * tan(fovHorizontal / 2.0));

    float dw = 1.0/WIDTH;
    float dh = 1.0/HEIGHT;

    float sampleOffsetX = 1.0/(samples*WIDTH);
    float sampleOffsetY = 1.0/(samples*HEIGHT);

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            float sx = (i) * dw;
            float sy = (j) * dh;
            Pos PixelCenterPosition = M + X*(2.0 * sx - 1.0) + Y * (2.0 * sy - 1.0);
            Pixel p = Pixel(samples, PixelCenterPosition);
            for(int sampleCountY = 0; sampleCountY < samples; sampleCountY++){
                for(int sampleCountX = 0; sampleCountX < samples; sampleCountX++){
                    Pos samplePosition = PixelCenterPosition
                        + X * sampleOffsetX * sampleCountX
                        + Y * sy * sampleOffsetY * sampleCountY;
                    Colr sample = Ray(E, samplePosition - E).trace(10);
                    p.samples.push_back(sample);
                }
            }
            pixels.push_back(p);
        }
    }
    filter();
    saveFile(filename, false);
}









