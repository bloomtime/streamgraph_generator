#pragma once

#include "LayerSort.h"

/**
 * NoLayerSort
 * Does no sorting. Identity function.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class NoLayerSort : public LayerSort {
public:
  std::string getName() {
    return "No Sorting";
  }

  LayerRefVec& sort(LayerRefVec& layers) {
    return layers;
  }

};
