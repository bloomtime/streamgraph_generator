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
      return (ascending ? pL->volatility < qL->volatility : pL->volatility > qL->volatility);
  }

//  bool equals(LayerRef pL, LayerRef qL) {
//    return pL->volatility == qL->volatility;
//  }

};
