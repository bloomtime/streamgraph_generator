#pragma once

#include "LayerLayout.h"
#include <algorithm>

/**
 * StackLayout
 * Standard stacked graph layout, with a straight baseline
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class StackLayout : public LayerLayout {
public:
  std::string getName() {
    return "Stacked Layout";
  }

  void layout(LayerRefVec& layers) {
    int n = layers[0]->size.size();

    // lay out layers, top to bottom.
    std::vector<float> baseline(n);
    std::fill(baseline.begin(), baseline.end(), 0);

    // Put layers on top of the baseline.
    stackOnBaseline(layers, baseline);
  }

};
