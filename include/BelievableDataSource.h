#pragma once

#include <algorithm>
#include "Layer.h"
#include "StreamDataSource.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"

/**
 * BelievableDataSource
 * Create test data for layout engine.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class BelievableDataSource : public StreamDataSource {
    
public:
  ci::Rand rnd;

  // seeded, so we can reproduce results
  BelievableDataSource(int seed=2): rnd(seed) {}

  LayerRefVec make(int numLayers, int sizeArrayLength) {
    LayerRefVec layers(numLayers);

    for (int i = 0; i < numLayers; i++) {
      std::string name = "Layer #" + ci::toString(i);
      std::vector<float> size(sizeArrayLength);
      makeRandomArray(size);
      layers[i] = Layer::create(name, size);
    }

    return layers;
  }

protected:

  void makeRandomArray(std::vector<float> &x) {
      
    std::fill(x.begin(), x.end(), 0.0f);

    // add a handful of random bumps
    for (int i=0; i<5; i++) {
      addRandomBump(x);
    }
  }

  void addRandomBump(std::vector<float> &x) {
    float height  = 1.0f / rnd.nextFloat();
    float cx      = 2.0f * rnd.nextFloat() - 0.5f;
    float r       = rnd.nextFloat() / 10.0f;

    for (int i = 0; i < x.size(); i++) {
      float a = ((float)i / (float)x.size() - cx) / r;
      x[i] += height * exp(-a * a);
    }
  }

};
