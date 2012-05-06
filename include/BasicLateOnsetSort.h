#pragma once

#include "LayerSort.h"
#include "OnsetComparator.h"

/**
 * BasicLateOnsetSort
 * Sorts by onset, but does not partition to the outsides of the graph in
 * order to illustrate short-sighted errors found during design process.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class BasicLateOnsetSort : public LayerSort {
public:

  static LayerSortRef create() {
    return LayerSortRef(new BasicLateOnsetSort());
  }
    
  std::string getName() {
    return "Late Onset Sorting, Top to Bottom";
  }

  void sort(LayerRefVec& layers) {
    // first sort by onset
    std::sort(layers.begin(), layers.end(), OnsetComparator(true));
  }

private:
    
    BasicLateOnsetSort() {}
    
};
