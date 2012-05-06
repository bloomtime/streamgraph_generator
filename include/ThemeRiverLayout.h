#pragma once

#include "LayerLayout.h"

/**
 * ThemeRiverLayout
 * Layout used by the authors of the ThemeRiver paper
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class ThemeRiverLayout : public LayerLayout {
public:

  static LayerLayoutRef create() {
    return LayerLayoutRef(new ThemeRiverLayout());
  }    
    
  std::string getName() {
    return "ThemeRiver";
  }

  void layout(LayerRefVec& layers) {
    // Set shape of baseline values.
    int n=layers[0]->size.size();
    int m=layers.size();
    std::vector<float> baseline(n);

    // ThemeRiver is perfectly symmetrical
    // the baseline is 1/2 of the total height at any point
    for (int i = 0; i < n; i++) {
      baseline[i] = 0;
      for (int j = 0; j < m; j++) {
        baseline[i] += layers[j]->size[i];
      }
      baseline[i] *= 0.5;
    }

    // Put layers on top of the baseline.
    stackOnBaseline(layers, baseline);
  }
    
private:

  ThemeRiverLayout() {}

};
