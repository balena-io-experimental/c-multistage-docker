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
// // #include <chrono>
// #include <time.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;

float scaleMultiply = 5.0;
float speedMultiply = 30.0;
int baseR = 255;
int baseG = 255;
int baseB = 255;
int gridX, gridY;
double k = 0;
unsigned long timer = 0;

FastNoise noise;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}

// static void DrawOnCanvas(Canvas *canvas) {
//   /*
//    * Let's create a simple animation. We use the canvas to draw
//    * pixels. We wait between each step to have a slower animation.
//    */
//   canvas->Fill(0, 0, 0);

//   int center_x = canvas->width() / 2;
//   int center_y = canvas->height() / 2;
//   float radius_max = canvas->width() / 2;
//   float angle_step = 1.0 / 360;
//   for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
//     if (interrupt_received)
//       return;
//     float dot_x = cos(a * 2 * M_PI) * r;
//     float dot_y = sin(a * 2 * M_PI) * r;
//     canvas->SetPixel(center_x + dot_x, center_y + dot_y,
//                      255, 0, 0);
//     usleep(1 * 1000);  // wait a little to slow down things.
//   }
// }

static void DrawOnCanvas(Canvas *canvas) {
    // auto now = std::chrono::steady_clock::now();
    // auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    // auto epoch = now_ms.time_since_epoch();
    // auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    // long duration = value.count();
  canvas->Fill(0, 0, 0);

  for(int i=0; i<canvas->width(); i++) {
    for(int j=0; j<canvas->height(); j++) {
      if (interrupt_received)
        return;
      int col = (int)((noise.GetNoise((float)i,(float)j,(float)(k++))+1.0)/2.0*100.0);
      canvas->SetPixel(i, j, col, col, col);
      usleep(1 * 1000);
    }
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

  noise.SetNoiseType(FastNoise::SimplexFractal);
  noise.SetSeed(1337);

  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  // DrawOnCanvas(canvas);    // Using the canvas.
  DrawOnCanvas(canvas);

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
