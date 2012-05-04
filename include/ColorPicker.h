#pragma once

#include "Layer.h"

/**
 * ColorPicker
 * Interface for new coloring algorithms.
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class ColorPicker {
public:
  virtual void colorize(LayerRefVec& layers) = 0;

  virtual std::string getName() = 0;

  virtual ~ColorPicker() {}
};
