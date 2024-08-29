#include "editor_layer.h"
#include <function/application/application.h>
#include <function/application/window.h>

int main(int argc, char const* argv[])
{
    Leaper::Application application(Leaper::RenderAPI::API::OpenGL, 1024, 648, "Leaper Engine");
    application.OnAttach();

    EditorLayer* editor_layer = new EditorLayer();
    application.PushOverlay(editor_layer);

    while (!application.GetWindow()->IsClose()) { application.Run(); }

    return 0;
}
