#pragma once

#include <string>
#include "cinder/Cinder.h"
#include "cinder/Color.h"

typedef std::shared_ptr<class Layer> LayerRef;
typedef std::vector<LayerRef>        LayerRefVec;

/**
 * Layer
 * Represents a layer in a layered graph, maintaining properties which
 * define it's position, size, color and mathemetical characteristics
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class Layer {
public:
  std::string   name;
  std::vector<float>  size;
  std::vector<float>  yBottom;
  std::vector<float>  yTop;
  ci::ColorA      rgb;
  int      onset;
  int      end;
  float    sum;
  float    volatility;

  static LayerRef create(std::string name, std::vector<float> size) {
    return LayerRef(new Layer(name,size));
  }
    
private:
  Layer(std::string _name, std::vector<float> _size):
    name(_name), 
    size(_size) {

    // check for reasonable data
    for (int i = 0; i < size.size(); i++) {
      if (size[i] < 0) {
        throw std::runtime_error("No negative sizes allowed.");
      }
    }

    yBottom.resize(size.size());
    yTop.resize(size.size());
    sum         = 0;
    volatility  = 0;
    onset       = -1;

    for (int i = 0; i < size.size(); i++) {

      // sum is the summation of all points
      sum += size[i];

      // onset is the first non-zero point
      // end is the last non-zero point
      if (size[i] > 0) {
        if (onset == -1) {
          onset = i;
        } else {
          end = i;
        }
      }

      // volatility is the maximum change between any two consecutive points
      if (i > 0) {
        volatility = fmax(
          volatility,
          fabs(size[i] - size[i-1])
        );
      }
    }
  }

};
