/**
 * (adapted from) streamgraph_generator
 * Processing Sketch
 * Explores different stacked graph layout, ordering and coloring methods
 * Used to generate example graphics for the Streamgraph paper
 *
 * Press Enter to save image
 *  
 * @author Lee Byron
 * @author Martin Wattenberg
 */

#pragma once

#include <vector>
#include "Layer.h"
#include "ColorPicker.h"
#include "LayerLayout.h"
#include "LayerSort.h"
#include "cinder/Path2d.h"
#include "cinder/gl/gl.h"
#include "cinder/Triangulate.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"

typedef std::shared_ptr<class StreamView> StreamViewRef;

class StreamView 
{    
public:
    
    static StreamViewRef create( std::shared_ptr<ci::Camera> camera, ci::gl::GlslProg shader, LayerRefVec layers, const ci::Vec2i &size, bool isGraphCurved=true )
    {
        return StreamViewRef( new StreamView( camera, shader, layers, size, isGraphCurved ) );
    }
    
    LayerRefVec layers;
    bool isGraphCurved; // catmull-rom interpolation
    ci::Vec2i size;
    int numLayers, layerSize;

    std::shared_ptr<ci::Camera> camera;
    ci::gl::GlslProg shader;
    
    // adding a pixel to the top compensate for antialiasing letting
    // background through. This is overlapped by following layers, so no
    // distortion is made to data.
    // detail: a pixel is not added to the top-most layer
    // detail: a shape is only drawn between it's non 0 values
    void draw()
    {
        int lastLayer = numLayers - 1;

        // generate graph
        for (int i = 0; i < numLayers; i++) {
            int start = fmax(0, layers[i]->onset - 1);
            int end   = fmin(layerSize - 1, layers[i]->end);
            
            // draw shape
            ci::Path2d path;
            
            // draw top edge, left to right
            graphVertex(path, start, layers[i]->yTop, isGraphCurved, i != lastLayer);
            for (int j = start; j <= end; j++) {
                graphVertex(path, j, layers[i]->yTop, isGraphCurved, i != lastLayer);
            }
            graphVertex(path, end, layers[i]->yTop, isGraphCurved, i != lastLayer);
            
            // draw bottom edge, right to left
            graphVertex(path, end, layers[i]->yBottom, isGraphCurved, false);
            for (int j = end; j >= start; j--) {
                graphVertex(path, j, layers[i]->yBottom, isGraphCurved, false);
            }
            graphVertex(path, start, layers[i]->yBottom, isGraphCurved, false);
            
            path.close();
            
            drawSolid( ci::Triangulator(path).calcMesh(), layers[i]->rgb );
        }        
    }
    
    void drawSolid( const ci::TriMesh2d &mesh, ci::ColorA color )
    {
        if( mesh.getNumVertices() <= 0 )
            return;
        
        ci::Matrix44f mat = camera->getProjectionMatrix() * camera->getModelViewMatrix();    

        shader.bind();
        shader.uniform("u_mvp_matrix", mat);    
        shader.uniform("u_color", color);
        
        int position = shader.getAttribLocation("position");
        
        glVertexAttribPointer(position, 2, GL_FLOAT, false, 0, &(mesh.getVertices()[0]));
        glEnableVertexAttribArray(position);
        
        glDrawElements( GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, &(mesh.getIndices()[0]) );
    }
    
private:

    void graphVertex(ci::Path2d &path, int point, std::vector<float>& source, bool curve, bool pxl) 
    {
        float x = ci::lerp(0.0f, (float)size.x, (float)point / (float)(layerSize - 1));
        float y = source[point] - (pxl ? 1 : 0);
        if (path.empty()) {
            path.moveTo( x, y );
        } else if (curve) {
            // FIXME: equivalent to Processing's curveVertex here?
            path.lineTo( x, y );
        } else {
            path.lineTo( x, y );
        }
    }
    
    void scaleLayers(LayerRefVec &layers, int screenTop, int screenBottom) 
    {
        // Figure out max and min values of layers.
        float min = layers[0]->yTop[0];
        float max = layers[0]->yBottom[0];
        for (int i = 0; i < layerSize; i++) {
            for (int j = 0; j < numLayers; j++) {
                min = fmin(min, layers[j]->yTop[i]);
                max = fmax(max, layers[j]->yBottom[i]);
            }
        }
        
        float scale = (float)(screenBottom - screenTop) / (max - min);
        for (int i = 0; i < layerSize; i++) {
            for (int j = 0; j < numLayers; j++) {
                layers[j]->yTop[i] = screenTop + scale * (layers[j]->yTop[i] - min);
                layers[j]->yBottom[i] = screenTop + scale * (layers[j]->yBottom[i] - min);
            }
        }
    }
    
    StreamView( std::shared_ptr<ci::Camera> _camera, ci::gl::GlslProg _shader, LayerRefVec _layers, const ci::Vec2i &_size, bool _isGraphCurved ):
        camera(_camera),
        shader(_shader),
        layers(_layers),
        size(_size),
        isGraphCurved(_isGraphCurved)
    {
        numLayers = layers.size();
        layerSize = layers[0]->size.size();
        
        // fit graph to viewport
        scaleLayers(layers, 1, size.y - 1);
    }
    
};