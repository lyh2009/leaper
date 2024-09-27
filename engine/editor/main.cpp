#include "editor_layer.h"
#include <function/application/application.h>
#include <function/application/window.h>

int main(int argc, char const* argv[])
{
    Leaper::Application application(Leaper::RenderAPI::API::OpenGL, 1024, 648, "Leaper Engine");
    application.OnAttach();

    Leaper::EditorLayer* editor_layer = new Leaper::EditorLayer();
    application.PushOverlay(editor_layer);

    application.Run();

    return 0;
}
