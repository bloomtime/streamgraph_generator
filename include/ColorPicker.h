#pragma once

#include "Layer.h"

typedef std::shared_ptr<class ColorPicker> ColorPickerRef;

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
