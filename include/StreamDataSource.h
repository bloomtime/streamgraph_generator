#pragma once

#include "Layer.h"

/**
 * StreamDataSource
 * Interface for creating a data source
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class StreamDataSource {

public:
    virtual LayerRefVec make(int numLayers, int sizeArrayLength) = 0;
    virtual ~StreamDataSource() {};
};
