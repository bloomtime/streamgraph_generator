#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Path2d.h"
#include "cinder/Camera.h"

#include "ColorPicker.h"
#include "LastFMColorPicker.h"
#include "RandomColorPicker.h"

#include "OnsetComparator.h"
#include "VolatilityComparator.h"

#include "StreamDataSource.h"
#include "BelievableDataSource.h"
#include "LateOnsetDataSource.h"

#include "LayerLayout.h"
#include "MinimizedWiggleLayout.h"
#include "StackLayout.h"
#include "StreamLayout.h"
#include "ThemeRiverLayout.h"

#include "LayerSort.h"
#include "BasicLateOnsetSort.h"
#include "InverseVolatilitySort.h"
#include "LateOnsetSort.h"
#include "NoLayerSort.h"
#include "VolatilitySort.h"

#include "Layer.h"

using namespace ci;
using namespace ci::app;

class StreamGraphGeneratorApp : public AppCocoaTouch {
  public:
	void setup();
	void draw();

    void graphVertex(int point, std::vector<float> &source, bool curve, bool pxl);
    void scaleLayers(LayerRefVec &layers, int screenTop, int screenBottom);
    
    bool        isGraphCurved; // catmull-rom interpolation
//    int         seed;          // random seed
    
//    float       DPI;
//    float       widthInches;
//    float       heightInches;
    int         numLayers;
    int         layerSize;
    
    StreamDataSource*  data;
    LayerLayout*       layout;
    LayerSort*         ordering;
    ColorPicker*       coloring;
    
    LayerRefVec layers;
};

/**
 * streamgraph_generator
 * Processing Sketch
 * Explores different stacked graph layout, ordering and coloring methods
 * Used to generate example graphics for the Streamgraph paper
 *
 * Press Enter to save image
 *  
 * @author Lee Byron
 * @author Martin Wattenberg
 */

void StreamGraphGeneratorApp::setup() {
    
    isGraphCurved = true; // catmull-rom interpolation
//    seed          = 28;   // random seed
    
//    DPI           = 300;
//    widthInches   = 3.5;
//    heightInches  = 0.7;
    numLayers     = 50;
    layerSize     = 100;    
    
//    size(int(widthInches*DPI), int(heightInches*DPI));
//    smooth();
//    noLoop();
    
    // GENERATE DATA
    data     = new LateOnsetDataSource();
    //data     = new BelievableDataSource();
    
    // ORDER DATA
    ordering = new LateOnsetSort();
    //ordering = new VolatilitySort();
    //ordering = new InverseVolatilitySort();
    //ordering = new BasicLateOnsetSort();
    //ordering = new NoLayerSort();
    
    // LAYOUT DATA
    layout   = new StreamLayout();
    //layout   = new MinimizedWiggleLayout();
    //layout   = new ThemeRiverLayout();
    //layout   = new StackLayout();
    
    // COLOR DATA
    coloring = new LastFMColorPicker("layers-nyt.jpg");
    //coloring = new LastFMColorPicker(this, "layers.jpg");
    //coloring = new RandomColorPicker(this);
    
    //=========================================================================
    
    // calculate time to generate graph
    double time = getElapsedSeconds();
    
    // generate graph
    layers = data->make(numLayers, layerSize);
    ordering->sort(layers);
    layout->layout(layers);
    coloring->colorize(layers);
    
    // fit graph to viewport
    scaleLayers(layers, 1, getWindowHeight() - 1);
    
    // give report
    double layoutTime = getElapsedSeconds() - time;
    int numLayers = layers.size();
    int layerSize = layers[0]->size.size();
    std::cout << "Data has " << numLayers << " layers, each with " <<
            layerSize << " datapoints." << std::endl;
    std::cout << "Layout Method: " << layout->getName() << std::endl;
    std::cout << "Ordering Method: " << ordering->getName() << std::endl;
    std::cout << "Coloring Method: " << layout->getName() << std::endl;
    std::cout << "Elapsed Time: " << layoutTime << "s" << std::endl;
    
    delete data;
    delete ordering;
    delete layout;
    delete coloring;
}

// adding a pixel to the top compensate for antialiasing letting
// background through. This is overlapped by following layers, so no
// distortion is made to data.
// detail: a pixel is not added to the top-most layer
// detail: a shape is only drawn between it's non 0 values
void StreamGraphGeneratorApp::draw() {
    
    int n = layers.size();
    int m = layers[0]->size.size();
    int start;
    int end;
    int lastIndex = m - 1;
    int lastLayer = n - 1;
    int pxl;
    
    gl::clear( Color::white() );
//    noStroke();
    
    // calculate time to draw graph
    double time = getElapsedSeconds();
    
    // generate graph
    for (int i = 0; i < n; i++) {
        start = fmax(0, layers[i]->onset - 1);
        end   = fmin(m - 1, layers[i]->end);
        pxl   = i == lastLayer ? 0 : 1;
        
        // set fill color of layer
        gl::color(layers[i]->rgb);
        
        // draw shape
        //beginShape();
        ci::Path2d path;
        
        // draw top edge, left to right
        graphVertex(start, layers[i]->yTop, isGraphCurved, i == lastLayer);
        for (int j = start; j <= end; j++) {
            graphVertex(j, layers[i]->yTop, isGraphCurved, i == lastLayer);
        }
        graphVertex(end, layers[i]->yTop, isGraphCurved, i == lastLayer);
        
        // draw bottom edge, right to left
        graphVertex(end, layers[i]->yBottom, isGraphCurved, false);
        for (int j = end; j >= start; j--) {
            graphVertex(j, layers[i]->yBottom, isGraphCurved, false);
        }
        graphVertex(start, layers[i]->yBottom, isGraphCurved, false);
        
        gl::draw( path );
//        endShape(CLOSE);
    }
    
    // give report
    double layoutTime = getElapsedSeconds() - time;
    std::cout << "Draw Time: " << layoutTime << "s" << std::endl;
}

void StreamGraphGeneratorApp::graphVertex(int point, std::vector<float>& source, bool curve, bool pxl) {
    float x = lerp(0.0f, (float)getWindowWidth(), (float)point / (float)(layerSize - 1));
    float y = source[point] - (pxl ? 1 : 0);
    if (curve) {
//        curveVertex(x, y);
    } else {
//        vertex(x, y);
    }
}

void StreamGraphGeneratorApp::scaleLayers(LayerRefVec &layers, int screenTop, int screenBottom) {
    // Figure out max and min values of layers.
    float min = layers[0]->yTop[0];
    float max = layers[0]->yBottom[0];
    for (int i = 0; i < layers[0]->size.size(); i++) {
        for (int j = 0; j < layers.size(); j++) {
            min = fmin(min, layers[j]->yTop[i]);
            max = fmax(max, layers[j]->yBottom[i]);
        }
    }
    
    float scale = (screenBottom - screenTop) / (max - min);
    for (int i = 0; i < layers[0]->size.size(); i++) {
        for (int j = 0; j < layers.size(); j++) {
            layers[j]->yTop[i] = screenTop + scale * (layers[j]->yTop[i] - min);
            layers[j]->yBottom[i] = screenTop + scale * (layers[j]->yBottom[i] - min);
        }
    }
}

//void keyPressed() {
//    if (keyCode == ENTER) {
//        println();
//        println("Rendering image...");
//        String fileName = "images/streamgraph-" + dateString() + ".png";
//        save(fileName);
//        println("Rendered image to: " + fileName);
//    }
//    
//    // hack for un-responsive non looping p5 sketches
//    if (keyCode == ESC) {
//        redraw();
//    }
//}
//
//String dateString() {
//    return year() + "-" + nf(month(), 2) + "-" + nf(day(), 2) + "@" +
//    nf(hour(), 2) + "-" + nf(minute(), 2) + "-" + nf(second(), 2);
//}

CINDER_APP_COCOA_TOUCH( StreamGraphGeneratorApp, RendererGl )
