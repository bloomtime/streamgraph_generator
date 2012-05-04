#pragma once

#include "Layer.h"

/**
 * OnsetSort
 * Compares two Layers based on their onset
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class OnsetComparator {
public:
  bool ascending;

  OnsetComparator(bool _ascending): ascending(_ascending) {}

  bool operator()(LayerRef pL, LayerRef qL) {
    return (ascending ? pL->onset < qL->onset : pL->onset < qL->onset);
  }

//  bool equals(LayerRef pL, LayerRef qL){
//    return pL->onset == qL->onset;
//  }

};
