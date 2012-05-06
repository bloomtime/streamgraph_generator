#pragma once

#include "LayerSort.h"
#include "VolatilityComparator.h"

/**
 * InverseVolatilitySort
 * Sorts an array of layers by their volatility, placing the most volatile
 * layers along the insides of the graph, illustrating how disruptive this
 * volatility can be to a stacked graph.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class InverseVolatilitySort : public LayerSort {

public:
    
  static LayerSortRef create() {
    return LayerSortRef(new InverseVolatilitySort());
  }
    
  std::string getName() {
    return "Inverse Volatility Sorting, Evenly Weighted";
  }

  void sort(LayerRefVec& layers) {
    // first sort by volatility
    std::sort(layers.begin(), layers.end(), VolatilityComparator(false));

    orderToOutside(layers);
  }
    
private:
    
  InverseVolatilitySort() {}

};
