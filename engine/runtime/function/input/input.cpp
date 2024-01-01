#include "lppch.h"
#include "input.h"
#include <platform/opengl/opengl_input.h>

Leaper::Input* Leaper::Input::m_input = new OpenGLInput();