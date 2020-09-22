// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "FastNoise.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;

int baseR = 255;
int baseG = 255;
int baseB = 255;
float globalBrightness = 50.0f;
float scale = 5.0f;
float speed = 1.0f;
float k = 0.0f;


static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas, FastNoise noise) {
  canvas->Fill(0, 0, 0);

  while (!interrupt_received) {
    for(float i=0.0f; i<canvas->width(); i+=1.0f) {
      for(float j=0.0f; j<canvas->height(); j+=1.0f) {
        float col = (noise.GetNoise(i*scale,j*scale,k*speed)+1.0f)/2.0f*globalBrightness;
        // int col = (int)((noise.GetNoise(i,j,k)+1.0)/2.0*100.0);
        canvas->SetPixel(i, j, col, col, col);
      }
    }
    k+=1.0f;
    usleep(15 * 1000);
  }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";
  defaults.rows = 32;
  defaults.cols = 64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  
  FastNoise noise;

  noise.SetNoiseType(FastNoise::Simplex);
  // noise.SetSeed(1337);

  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  // DrawOnCanvas(canvas);    // Using the canvas.
  while(!interrupt_received)
    DrawOnCanvas(canvas, noise);

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  printf("\%s. Exiting.\n",
         interrupt_received ? "Received CTRL-C" : "Timeout reached");

  return 0;
}
