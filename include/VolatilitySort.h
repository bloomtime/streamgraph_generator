#pragma once

#include "LayerSort.h"
#include "VolatilityComparator.h"

/**
 * VolatilitySort
 * Sorts an array of layers by their volatility, placing the most volatile
 * layers along the outsides of the graph, thus minimizing unneccessary
 * distortion.
 *
 * First sort by volatility, then creates a 'top' and 'bottom' collection.
 * Iterating through the sorted list of layers, place each layer in whichever
 * collection has less total mass, arriving at an evenly weighted graph.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class VolatilitySort : public LayerSort {

public:
  std::string getName() {
    return "Volatility Sorting, Evenly Weighted";
  }

  void sort(LayerRefVec& layers) {
    // first sort by volatility
    std::sort(layers.begin(), layers.end(), VolatilityComparator(true));
    orderToOutside(layers);
  }

};
