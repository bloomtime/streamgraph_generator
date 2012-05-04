#pragma once

#include "ColorPicker.h"
#include "cinder/Surface.h"
#include "cinder/ImageIO.h"
#include "cinder/app/App.h"

/**
 * LastFMColorPicker
 * Loads in an image and uses it as a two-dimensional gradient
 * Supply two [0,1) numbers and get the color of the gradient at that point
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LastFMColorPicker : public ColorPicker {

public:
  ci::Surface source;

  LastFMColorPicker(std::string src) {
    source = ci::loadImage(ci::app::loadResource(src));
  }

  std::string getName() {
    return "Listening History Color Scheme";
  }

  void colorize(LayerRefVec& layers) {
    // find the largest layer to use as a normalizer
    float maxSum = 0;
    for (int i=0; i<layers.size(); i++) {
      maxSum = fmax(maxSum, layers[i]->sum);
    }

    // find the color for each layer
    for (int i = 0; i < layers.size(); i++) {
      float normalizedOnset = (float)layers[i]->onset / layers[i]->size.size();
      float normalizedSum = layers[i]->sum / maxSum;
      float shapedSum = (float)(1.0 - sqrt(normalizedSum));

      layers[i]->rgb = get(normalizedOnset, shapedSum);
    }
  }

protected:
  ci::ColorA get(float g1, float g2) {
    // get pixel coordinate based on provided parameters
    int x = floor(g1 * source.getWidth());
    int y = floor(g2 * source.getHeight());

    // ensure that the pixel is within bounds.
    x = ci::constrain(x, 0, source.getWidth() - 1);
    y = ci::constrain(y, 0, source.getHeight() - 1);

    // return the color at the requested pixel
    return source.getPixel(ci::Vec2i(x,y));
  }

};
