#include <iostream>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "win.h"


template <typename ResourceTag, typename ResourceType>
class Resource
{
public:
	Resource();
	
	Resource(const Resource&) = delete;
	Resource& operator=(const Resource&) = delete;

	ResourceType get() { return resource; }

private:
	ResourceType resource{};
};

template <typename ResourceType>
auto& GetResource()
{
	static ResourceType resource;
	return resource;
}

using Display = Resource<struct DisplayTag, EGLDisplay>;
template<> Display::Resource()
{
	resource = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (resource == EGL_NO_DISPLAY)
	{
		throw std::runtime_error("Unable to get display");
	}

	EGLint major;
	EGLint minor;
	if (EGL_FALSE == eglInitialize(resource, &major, &minor))
	{
		throw std::runtime_error("Unable to initialize OpenGL");
	}
}

using Config = Resource<struct ConfigTag, EGLConfig>;
template<> Config::Resource()
{
	EGLint numConfig;

	EGLint configAttribs[] =
	{
		EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE,   8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE,  8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};

	EGLDisplay display = GetResource<Display>().get();

	if (EGL_FALSE == eglChooseConfig(display, configAttribs, &resource, 1, &numConfig))
	{
		throw std::runtime_error("Unable to choose config");
	}
}

EGLSurface CreateSurface(EGLDisplay _display, EGLNativeWindowType _window)
{
	EGLConfig config = GetResource<Config>().get();
	
	EGLSurface surface = eglCreateWindowSurface(_display, config, _window, NULL);

	if (EGL_NO_SURFACE == surface)
	{
		switch (eglGetError())
		{
		case EGL_BAD_MATCH:
			break;
		case EGL_BAD_CONFIG:
			break;
		case EGL_BAD_NATIVE_WINDOW:
			break;
		case EGL_BAD_ALLOC:
			break;
		}
	}
	
	return surface;
}

EGLContext CreateContext(EGLDisplay _display, EGLConfig _config)
{
	const EGLint attribList[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLContext context = eglCreateContext(_display, _config, EGL_NO_CONTEXT, attribList);

	if (EGL_NO_CONTEXT == context)
	{
		EGLint error = eglGetError();

		if (EGL_BAD_CONFIG == error)
		{
			std::cout << "CreateContext : bad config" << std::endl;
		}

		return nullptr;
	}

	return context;
}


int main()
{
	window win("Hello!", 320, 240);
	win.show();

	HWND hWnd = win.handle();
	EGLDisplay display = GetResource<Display>().get();
	EGLSurface surface = CreateSurface(display, (EGLNativeWindowType)GetDC(hWnd));

	EGLint contextAttribs[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLConfig  config  = GetResource<Config>().get();
	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

	eglMakeCurrent(display, surface, surface, context);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}