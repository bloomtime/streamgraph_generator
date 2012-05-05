#pragma once

#include "StreamDataSource.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"

/**
 * LateOnsetData
 * Creates false data which resembles late onset time-series.
 * Such as band popularity or movie box-office income.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LateOnsetDataSource : public StreamDataSource {
public:
  ci::Rand rnd;

  // default seeded, so we can reproduce results
  LateOnsetDataSource(int seed=2): rnd(seed) {}

  LayerRefVec make(int numLayers, int sizeArrayLength) {
    LayerRefVec layers(numLayers);

    for (int i = 0; i < numLayers; i++) {
      std::string name = "Layer #" + ci::toString(i);
      int onset    = (int)((float)sizeArrayLength * (rnd.nextFloat() * 1.25f - 0.25f));
      int duration = (int)(rnd.nextFloat() * 0.75f * (float)sizeArrayLength);
      std::vector<float> size(sizeArrayLength, 0.0f);
      makeRandomArray(size, onset, duration);
      layers[i] = Layer::create(name, size);
    }

    return layers;
  }

protected:
  void makeRandomArray(std::vector<float> &x, int onset, int duration) {

    // add a single random bump
    addRandomBump(x, onset, duration);
  }

  void addRandomBump(std::vector<float> &x, int onset, int duration) {
    float height  = rnd.nextFloat();
    int start     = fmax(0, onset);
//    int end       = fmin(x.size(), onset + duration);
//    int len       = end - onset;

    for (int i = start; i < x.size() && i < onset + duration; i++) {
      float xx = (float)(i - onset) / (float)duration;
      float yy = (float)(xx * exp(-10.0f * xx));
      x[i] += height * yy;
    }
  }

};
