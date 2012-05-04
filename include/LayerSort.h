#pragma once

#include <string>
#include <vector>
#include "cinder/Cinder.h"
#include "Layer.h"

/**
 * LayerSort
 * Interface to sorting layers
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class LayerSort {

public:
  virtual std::string getName() = 0;

  virtual void sort(LayerRefVec& layers) = 0;

  virtual ~LayerSort() {}
    
protected:
  /**
   * Creates a 'top' and 'bottom' collection.
   * Iterating through the previously sorted list of layers, place each layer
   * in whichever collection has less total mass, arriving at an evenly
   * weighted graph. Reassemble such that the layers that appeared earliest
   * end up in the 'center' of the graph.
   */
  void orderToOutside(LayerRefVec& layers) {
    int j             = 0;
    int n             = layers.size();
    LayerRefVec newLayers(n);
    int topCount      = 0;
    float topSum      = 0;
    std::vector<int> topList(n);
    int botCount      = 0;
    float botSum      = 0;
    std::vector<int> botList(n);

    // partition to top or bottom containers
    for (int i=0; i<n; i++) {
      if (topSum < botSum) {
        topList[topCount++] = i;
        topSum += layers[i]->sum;
      } else {
        botList[botCount++] = i;
        botSum += layers[i]->sum;
      }
    }

    // reassemble into single array
    for (int i = botCount - 1; i >= 0; i--) {
      newLayers[j++] = layers[botList[i]];
    }
    for (int i = 0; i < topCount; i++) {
      newLayers[j++] = layers[topList[i]];
    }
    
    // lots of copies but should be fast enough with shared_ptrs
    layers = newLayers;
  }

};
