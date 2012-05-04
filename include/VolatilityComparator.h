#pragma once

#include "Layer.h"

/**
 * VolatilityComparator
 * Compares two Layers based on their volatility
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class VolatilityComparator {
public:

  bool ascending;

  VolatilityComparator(bool _ascending): ascending(_ascending) {}

  bool operator()(LayerRef pL, LayerRef qL) {
    return (ascending ? pL->volatility < qL->volatility : pL->volatility > qL->volatility)
  }

//  public boolean equals(Object p, Object q) {
//    Layer pL = (Layer)p;
//    Layer qL = (Layer)q;
//    return pL.volatility == qL.volatility;
//  }

};
