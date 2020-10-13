#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "software renderer.h"

#include "GraphicsNode.h"

#include "MathLib.h"
#include "meshresource.h"
#include "textureresource.h"
#include "Shader.h"
#include "Footman.h"

namespace Example
{
	class ExampleApp : public Core::App
	{
	public:

		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		/// open app
		bool Open();
		/// run app
		void Run();

	private:

		Display::Window* window;

		Footman soldier;

		GraphicsNode node;
		
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	};
}