#pragma once

#include "Layer.h"

typedef std::shared_ptr<class StreamDataSource> StreamDataSourceRef;

/**
 * StreamDataSource
 * Interface for creating a data source
 * (was DataSource in original Processing code, renamed for Cinder compat.) 
 *
 * @author Lee Byron
 * @author Martin Wattenberg
 */
class StreamDataSource {

public:
    virtual LayerRefVec make(int numLayers, int sizeArrayLength) = 0;
    virtual ~StreamDataSource() {};
};
