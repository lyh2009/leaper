#include "launcher.h"
#include <function/application/application.h>
#include <function/application/window.h>

int main(int argc, char const* argv[])
{
    Leaper::Application application(Leaper::RenderAPI::API::OpenGL, 1024, 648, "Leaper Engine");

    Leaper::Launcher* launcher = new Leaper::Launcher();
    application.PushOverlay(launcher);
    application.OnAttach();

    while (!application.GetWindow()->IsClose()) { application.Run(); }

    return 0;
}
