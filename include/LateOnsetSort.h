#pragma once

#include "LayerSort.h"
#include "OnsetComparator.h"

/**
 * LateOnsetSort
 * Sorts by onset, and orders to the outsides of the graph.
 *
 * This is the sort technique preferred when using late-onset data, which the
 * Streamgraph technique is best suited to represent
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LateOnsetSort : public LayerSort {
public:

  static LayerSortRef create() {
    return LayerSortRef(new LateOnsetSort());
  }
    
  std::string getName() {
    return "Late Onset Sorting, Evenly Weighted";
  }

  void sort(LayerRefVec& layers) {
    // first sort by onset
    std::sort(layers.begin(), layers.end(), OnsetComparator(true));
    orderToOutside(layers);
  }

private:

  LateOnsetSort() {}

};
