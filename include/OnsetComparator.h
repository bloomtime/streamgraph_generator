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

//  public boolean equals(Object p, Object q){
//    Layer pL = (Layer)p;
//    Layer qL = (Layer)q;
//    return pL.onset == qL.onset;
//  }

}
