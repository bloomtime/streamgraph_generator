#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"

#include "StreamView.h"

// pick one:
#include "LastFMColorPicker.h"
#include "RandomColorPicker.h"

// pick one:
#include "BelievableDataSource.h"
#include "LateOnsetDataSource.h"

// pick one:
#include "MinimizedWiggleLayout.h"
#include "StackLayout.h"
#include "StreamLayout.h"
#include "ThemeRiverLayout.h"

// pick one:
#include "BasicLateOnsetSort.h"
#include "InverseVolatilitySort.h"
#include "LateOnsetSort.h"
#include "NoLayerSort.h"
#include "VolatilitySort.h"

using namespace ci;
using namespace ci::app;

class StreamGraphGeneratorApp : public AppCocoaTouch
{
public:

	void setup();
	void draw();

    StreamViewRef streamView;
    
};

void StreamGraphGeneratorApp::setup()
{
    // calculate time to generate graph
    double time = getElapsedSeconds();
    
    // GENERATE DATA
    //StreamDataSourceRef data = LateOnsetDataSource::create(seed);
    StreamDataSourceRef data = BelievableDataSource::create();
    
    // ORDER DATA
    LayerSortRef ordering = LateOnsetSort::create();
    //LayerSortRef ordering = VolatilitySort::create();
    //LayerSortRef ordering = InverseVolatilitySort::create();
    //LayerSortRef ordering = BasicLateOnsetSort::create();
    //LayerSortRef ordering = NoLayerSort::create();
    
    // LAYOUT DATA
    LayerLayoutRef layout = StreamLayout::create();
    //LayerLayoutRef layout = MinimizedWiggleLayout::create();
    //LayerLayoutRef layout = ThemeRiverLayout::create();
    //LayerLayoutRef layout = StackLayout::create();
    
    // COLOR DATA
    //ColorPickerRef coloring = LastFMColorPicker::create("layers-nyt.jpg");
    ColorPickerRef coloring = LastFMColorPicker::create("layers.jpg");
    //ColorPickerRef coloring = RandomColorPicker::create();    

    // numLayers, layerSize
    LayerRefVec layers = data->make(50, 100);
    
    // generate graph
    ordering->sort(layers);
    layout->layout(layers);
    coloring->colorize(layers);
    
    // create the view
    streamView = StreamView::create( layers, getWindowSize() );
    
    // give report
    double layoutTime = getElapsedSeconds() - time;
    std::cout << "Data has " << layers.size() << " layers, each with " <<
    layers[0]->size.size() << " datapoints." << std::endl;
    std::cout << "Layout Method: " << layout->getName() << std::endl;
    std::cout << "Ordering Method: " << ordering->getName() << std::endl;
    std::cout << "Coloring Method: " << layout->getName() << std::endl;
    std::cout << "Elapsed Time: " << layoutTime << "s" << std::endl;    
}

void StreamGraphGeneratorApp::draw() {

    // calculate time to draw graph
    double time = getElapsedSeconds();
    
    gl::clear( Color::white() );
    gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
    
    gl::enableAlphaBlending();
    gl::disableDepthRead();
    gl::disableDepthWrite();
    
    streamView->draw();
            
    // give report
    double layoutTime = getElapsedSeconds() - time;
    std::cout << "Draw Time: " << layoutTime << "s" << std::endl;
}

CINDER_APP_COCOA_TOUCH( StreamGraphGeneratorApp, RendererGl )
