#pragma once

#include "DataSource.h"
#include "cinder/Rand.h"

/**
 * LateOnsetData
 * Creates false data which resembles late onset time-series.
 * Such as band popularity or movie box-office income.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LateOnsetDataSource : public DataSource {
public:
  ci::Rand rnd;

  // default seeded, so we can reproduce results
  LateOnsetDataSource(int seed=2): rnd(seed) {}

  LayerRefVec make(int numLayers, int sizeArrayLength) {
    LayerRefVec layers(numLayers);

    for (int i = 0; i < numLayers; i++) {
      std::string name   = "Layer #" + i;
      int onset     = (int)(sizeArrayLength * (rnd.nextFloat() * 1.25 - 0.25));
      int duration  = (int)(rnd.nextFloat() * 0.75 * sizeArrayLength);
      std::vector<float> size(sizeArrayLength);
      makeRandomArray(size, onset, duration);
      layers[i]     = Layer::create(name, size);
    }

    return layers;
  }

protected:
  void makeRandomArray(std::vector<float> &x, int onset, int duration) {
    std::fill(x.begin(), x.end(), 0.0f);

    // add a single random bump
    addRandomBump(x, onset, duration);
  }

  void addRandomBump(std::vector<float> &x, int onset, int duration) {
    float height  = rnd.nextFloat();
    int start     = fmax(0, onset);
    int end       = min(x.size(), onset + duration);
    int len       = end - onset;

    for (int i = start; i < x.size() && i < onset + duration; i++) {
      float xx = (float)(i - onset) / duration;
      float yy = (float)(xx * exp(-10 * xx));
      x[i] += height * yy;
    }
  }

};
