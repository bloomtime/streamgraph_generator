#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"

#include "ColorPicker.h"
#include "LastFMColorPicker.h"
#include "RandomColorPicker.h"

#include "OnsetComparator.h"
#include "VolatilityComparator.h"

#include "DataSource.h"
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
	virtual void	setup();
	virtual void	resize( ResizeEvent event );
	virtual void	update();
	virtual void	draw();
	virtual void	mouseDown( MouseEvent event );
		
	Matrix44f	mCubeRotation;
	gl::Texture mTex;
	CameraPersp	mCam;
};

void StreamGraphGeneratorApp::setup()
{
	mCubeRotation.setToIdentity();

	// Create a blue-green gradient as an OpenGL texture
	Surface8u surface( 256, 256, false );
	Surface8u::Iter iter = surface.getIter();
	while( iter.line() ) {
		while( iter.pixel() ) {
			iter.r() = 0;
			iter.g() = iter.x();
			iter.b() = iter.y();
		}
	}
	
	mTex = gl::Texture( surface );
}

void StreamGraphGeneratorApp::resize( ResizeEvent event )
{
	mCam.lookAt( Vec3f( 3, 2, -3 ), Vec3f::zero() );
	mCam.setPerspective( 60, event.getAspectRatio(), 1, 1000 );
}

void StreamGraphGeneratorApp::mouseDown( MouseEvent event )
{
	console() << "Mouse down @ " << event.getPos() << std::endl;
}

void StreamGraphGeneratorApp::update()
{
	mCubeRotation.rotate( Vec3f( 1, 1, 1 ), 0.03f );
}

void StreamGraphGeneratorApp::draw()
{
	gl::clear( Color( 0.2f, 0.2f, 0.3f ) );
	gl::enable( GL_TEXTURE_2D );
	gl::enableDepthRead();
	
	mTex.bind();
	gl::setMatrices( mCam );
	glPushMatrix();
		gl::multModelView( mCubeRotation );
		gl::drawCube( Vec3f::zero(), Vec3f( 2.0f, 2.0f, 2.0f ) );
	glPopMatrix();
}

CINDER_APP_COCOA_TOUCH( StreamGraphGeneratorApp, RendererGl )
