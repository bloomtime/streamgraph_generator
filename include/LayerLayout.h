#pragma once

#include <string>
#include "Layer.h"

typedef std::shared_ptr<class LayerLayout> LayerLayoutRef;

/**
 * LayerLayout
 * Abstract Class for new stacked graph layout algorithms
 *
 * Note: you do not need to worry about scaling to screen dimensions.
 * The display applet will do that automatically for you.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LayerLayout {
public:

  virtual void layout(LayerRefVec& layers) = 0;

  virtual std::string getName() = 0;

  virtual ~LayerLayout() {}

protected:
  /**
   * We define our stacked graphs by layers atop a baseline.
   * This method does the work of assigning the positions of each layer in an
   * ordered array of layers based on an initial baseline.
   */
  void stackOnBaseline(LayerRefVec& layers, std::vector<float>& baseline) {
    // Put layers on top of the baseline.      
    for (int i = 0; i < layers.size(); i++) {
      layers[i]->yBottom = baseline;
      for (int j = 0; j < baseline.size(); j++) {
        baseline[j] -= layers[i]->size[j];
      }
      layers[i]->yTop = baseline;
    }
  }

};
