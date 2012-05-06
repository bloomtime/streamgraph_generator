#pragma once

#include "ColorPicker.h"
#include "cinder/Rand.h"

/**
 * RandomColorPicker
 * Chooses random colors within an acceptable HSB color spectrum
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class RandomColorPicker : public ColorPicker {

public:

  ci::Rand rnd;

  // seeded, so we can reproduce results
  static ColorPickerRef create(int seed=2) {
    return ColorPickerRef(new RandomColorPicker(seed));
  }    
    
  std::string getName() {
    return "Random Colors";
  }

  void colorize(LayerRefVec& layers) {
    for (int i = 0; i < layers.size(); i++) {
      float h = ci::lerp(0.6f, 0.65f, rnd.nextFloat());
      float s = ci::lerp(0.2f, 0.25f, rnd.nextFloat());
      float b = ci::lerp(0.4f, 0.95f, rnd.nextFloat());

      layers[i]->rgb = hsb2rgb(h, s, b);
    }
  }

protected:

  ci::ColorA hsb2rgb(float x, float y, float z) {
    float calcR = 0;
    float calcG = 0;
    float calcB = 0;
    float calcA = 1;

    if (y == 0) { // saturation == 0
      calcR = calcG = calcB = z;
    } else {
      float which = (x - (int)x) * 6.0f;
      float f = which - (int)which;
      float p = z * (1.0f - y);
      float q = z * (1.0f - y * f);
      float t = z * (1.0f - (y * (1.0f - f)));

      switch ((int)which) {
        case 0:
          calcR = z;
          calcG = t;
          calcB = p;
          break;
        case 1:
          calcR = q;
          calcG = z;
          calcB = p;
          break;
        case 2:
          calcR = p;
          calcG = z;
          calcB = t;
          break;
        case 3:
          calcR = p;
          calcG = q;
          calcB = z;
          break;
        case 4:
          calcR = t;
          calcG = p;
          calcB = z;
          break;
        case 5:
          calcR = z;
          calcG = p;
          calcB = q;
          break;
      }
    }

    int calcRi = (int)(255 * calcR);
    int calcGi = (int)(255 * calcG);
    int calcBi = (int)(255 * calcB);
    int calcAi = (int)(255 * calcA);
    
    return ci::ColorA8u(calcRi, calcGi, calcBi, calcAi);
  }

private:

    RandomColorPicker(int seed):rnd(seed) {}
    
};
