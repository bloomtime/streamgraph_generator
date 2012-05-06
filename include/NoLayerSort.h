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

  static LayerSortRef create() {
    return LayerSortRef(new NoLayerSort());
  }    
    
  std::string getName() {
    return "No Sorting";
  }

  void sort(LayerRefVec& layers) {}

private:
    
  NoLayerSort() {}
    
};
