#pragma once

#include "OpenGL.h"
#include "IRasterOps.h"

namespace Crawfis
{
	namespace Graphics 
	{
		//
		// Concrete implementation of IRasterOps.
		//   - Standard depth testing.
		//   - no alpha testing
		//   - no stencil support
		//
		class NoDepthRasterOps : public IRasterOps
		{
		public:
			//
			// Set the raster opertations (depth testing, alpha testing, ...).
			// This is typically part of an IMaterial and should be called before an
			// IDrawable draws itself.
			//
			virtual void Enable()
			{
				//glDisable( GL_DEPTH_TEST );
				glDepthMask( GL_FALSE );
			}
			//
			// Reset the raster opertations (depth testing, alpha testing, ...). 
			// This is typically part of an IMaterial and should be called before an
			// IDrawable draws itself.

			//
			virtual void Disable()
			{
				//glDisable( GL_DEPTH_TEST );
				glDepthMask( GL_TRUE );
			}
		};
	}
}
