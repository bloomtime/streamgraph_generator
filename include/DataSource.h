#pragma once

#include "Layer.h"

/**
 * DataSource
 * Interface for creating a data source
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class DataSource {

public:
    virtual LayerRefVec make(int numLayers, int sizeArrayLength) = 0;

};
