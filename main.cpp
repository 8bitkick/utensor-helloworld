#include <stdio.h>

#include <cmath>
#include <iostream>

#include "input_image.h"  // contains the first sample taken from the MNIST test set
#include "mbed.h"
#include "models/my_model/my_model.hpp"  // generated model file
#include "uTensor.h"

using namespace uTensor;

int argmax(const Tensor &logits) {
  uint32_t num_elems = logits->num_elems();
  float max_value = static_cast<float>(logits(0));
  int max_index = 0;
  for (int i = 1; i < num_elems; ++i) {
    float value = static_cast<float>(logits(i));
    if (value >= max_value) {
      max_value = value;
      max_index = i;
    }
  }
  return max_index;
}

Serial pc(USBTX, USBRX, 115200);  // baudrate := 115200

int main(void) {
  printf("Simple MNIST end-to-end uTensor cli example (device)\n");

  static My_model model;

  // create the input/output tensor
  Tensor input_image = new RomTensor({1, 28, 28, 1}, flt, arr_input_image);
  Tensor logits = new RamTensor({1, 10}, flt);

  model.set_inputs({{My_model::input_0, input_image}})
      .set_outputs({{My_model::output_0, logits}})
      .eval();

  int max_index = argmax(logits);
  input_image.free();
  logits.free();

  printf("pred label: %d\r\n", max_index);

  return 0;
}
