#pragma once

#include "LayerLayout.h"

/**
 * MinimizedWiggleLayout
 * Minimizes the sum of squares of the layer slopes at each value
 *
 * We present this as a reasonable alternative to the Stream Graph for
 * real-time use. While it has some drawbacks compared to StreamLayout, it is
 * much faster to execute and is reasonable for real-time applications.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class MinimizedWiggleLayout : public LayerLayout {
public:
    
  static LayerLayoutRef create() {
    return LayerLayoutRef(new MinimizedWiggleLayout());
  }
    
  std::string getName() {
    return "Minimized Wiggle Layout";
  }

  void layout(LayerRefVec& layers) {
    int n             = layers[0]->size.size();
    int m             = layers.size();
    std::vector<float> baseline(n);

    // Set shape of baseline values.
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        baseline[i] += (m - j - 0.5) * layers[j]->size[i];
      }
      baseline[i] /= m;
    }

    // Put layers on top of the baseline.
    stackOnBaseline(layers, baseline);
  }

private:
    
    MinimizedWiggleLayout() {}
};
