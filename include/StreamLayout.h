#pragma once

#include "LayerLayout.h"

/**
 * StreamLayout
 * The layout used in the Streamgraph stacked graph
 *
 * Because this layout is using numeric integration, it is likely insufficient
 * for real-time display, especially for larger data sets.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class StreamLayout : public LayerLayout {

public:
    
  static LayerLayoutRef create() {
    return LayerLayoutRef(new StreamLayout());
  }
    
  std::string getName() {
    return "Original Streamgraph Layout";
  }

  void layout(LayerRefVec& layers) {
    int n             = layers[0]->size.size();
    int m             = layers.size();
    std::vector<float> baseline(n,0.0f);
    std::vector<float> center(n,0.0f);
    float totalSize = 0.0f;
    float moveUp = 0.0f;
    float increase = 0.0f;
    float belowSize = 0.0f;

    // Set shape of baseline values.
    for (int i = 0; i < n; i++) {
      // the 'center' is a rolling point. It is initialized as the previous
      // iteration's center value
      center[i] = i == 0 ? 0 : center[i-1];

      // find the total size of all layers at this point
      totalSize = 0;
      for (int j = 0; j < m; j++) {
        totalSize += layers[j]->size[i];
      }

      // account for the change of every layer to offset the center point
      for (int j = 0; j < m; j++) {
        if (i == 0) {
          increase = layers[j]->size[i];
          moveUp = 0.5f;
        } else {
          belowSize = 0.5f * layers[j]->size[i];
          for (int k = j + 1; k < m; k++) {
            belowSize += layers[k]->size[i];
          }
          increase = layers[j]->size[i] - layers[j]->size[i - 1];
          moveUp = totalSize == 0 ? 0.0f : (belowSize / totalSize);
        }
        center[i] += (moveUp - 0.5f) * increase;
      }

      // set baseline to the bottom edge according to the center line
      baseline[i] = center[i] + 0.5f * totalSize;
    }

    // Put layers on top of the baseline.
    stackOnBaseline(layers, baseline);
  }

private:
    
  StreamLayout() {}

};
