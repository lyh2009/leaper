#include "lppch.h"
#include "lua_globals_binder.h"


#include "core/time.h"
#include "function/ecs/scene.h"
#include "function/input/input.h"
#include "function/input/key_codes.h"
#include "function/input/mouse_codes.h"


void Leaper::LuaGlobalsBinder::BindGlobals(sol::state& state)
{
    state.create_named_table("Time");
    state["Time"]["GetDeltaTime"] = &Time::GetDeltaTime;

    state.new_enum<Leaper::Key>("Key", { { "Space", Leaper::Key::Space }, { "A", Leaper::Key::A },   { "B", Leaper::Key::B },      { "C", Leaper::Key::C },
                                         { "D", Leaper::Key::D },         { "E", Leaper::Key::E },   { "F", Leaper::Key::F },      { "G", Leaper::Key::G },
                                         { "H", Leaper::Key::H },         { "I", Leaper::Key::I },   { "J", Leaper::Key::J },      { "K", Leaper::Key::K },
                                         { "L", Leaper::Key::L },         { "M", Leaper::Key::M },   { "N", Leaper::Key::N },      { "O", Leaper::Key::O },
                                         { "P", Leaper::Key::P },         { "Q", Leaper::Key::Q },   { "R", Leaper::Key::R },      { "S", Leaper::Key::S },
                                         { "T", Leaper::Key::T },         { "U", Leaper::Key::U },   { "V", Leaper::Key::V },      { "W", Leaper::Key::W },
                                         { "X", Leaper::Key::X },         { "Y", Leaper::Key::Y },   { "Z", Leaper::Key::Z },      { "Right", Leaper::Key::Right },
                                         { "Left", Leaper::Key::Left },   { "UP", Leaper::Key::Up }, { "Down", Leaper::Key::Down } });

    state.create_named_table("Input");
    state["Input"]["IsKeyDown"]    = &Input::IsKeyDown;
    state["Input"]["IsKeyRelease"] = &Input::IsKeyRelease;
    state["Input"]["IsMouseDown"]  = &Input::IsMouseDown;
}