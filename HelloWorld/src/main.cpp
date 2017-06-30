#include <iostream>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "win.h"


EGLDisplay InitEGL()
{
	EGLint major;
	EGLint minor;
	EGLDisplay display;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (EGL_NO_DISPLAY == display)
	{
		std::cout << "Unable to open connection to loccal windowing system" << std::endl;
	}

	if (EGL_FALSE == eglInitialize(display, &major, &minor))
	{
		std::cout << "Unable to initialize EGL" << std::endl;
	}

	return display;
}

EGLSurface CreateSurface(EGLDisplay _display, EGLNativeWindowType _window)
{
	const EGLint MaxConfigs = 10;
	EGLConfig configs[MaxConfigs];

	EGLint attribList[] = 
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE,   5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE,  5,
		EGL_DEPTH_SIZE, 1,
		EGL_NONE
	};

	EGLint numConfigs;
	if (EGL_FALSE == eglChooseConfig(_display, attribList, configs, MaxConfigs, &numConfigs))
	{
		std::cout << "Unable to choose config" << std::endl;
	}
	else
	{
		EGLSurface surface;
		EGLint attribList[] = 
		{
			EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
			EGL_NONE
		};

		surface = eglCreateWindowSurface(_display, configs[0], _window, NULL);

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
		else return surface;
	}
	return nullptr;
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
	EGLDisplay display = InitEGL();
	EGLSurface surface = CreateSurface(display, (EGLNativeWindowType)GetDC(hWnd));


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}