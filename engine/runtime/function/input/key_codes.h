#pragma once
#include <stdint.h>

namespace Leaper
{
    using KeyCode = uint16_t;
    
    enum class Key : KeyCode
    {
        // From glfw3.h
        Space               = 32,
        Apostrophe          = 39, /* ' */
        Comma               = 44, /* , */
        Minus               = 45, /* - */
        Period              = 46, /* . */
        Slash               = 47, /* / */
        D0                  = 48, /* 0 */
        D1                  = 49, /* 1 */
        D2                  = 50, /* 2 */
        D3                  = 51, /* 3 */
        D4                  = 52, /* 4 */
        D5                  = 53, /* 5 */
        D6                  = 54, /* 6 */
        D7                  = 55, /* 7 */
        D8                  = 56, /* 8 */
        D9                  = 57, /* 9 */
        Semicolon           = 59, /* ; */
        Equal               = 61, /* = */
        A                   = 65,
        B                   = 66,
        C                   = 67,
        D                   = 68,
        E                   = 69,
        F                   = 70,
        G                   = 71,
        H                   = 72,
        I                   = 73,
        J                   = 74,
        K                   = 75,
        L                   = 76,
        M                   = 77,
        N                   = 78,
        O                   = 79,
        P                   = 80,
        Q                   = 81,
        R                   = 82,
        S                   = 83,
        T                   = 84,
        U                   = 85,
        V                   = 86,
        W                   = 87,
        X                   = 88,
        Y                   = 89,
        Z                   = 90,
        LeftBracket         = 91,  /* [ */
        Backslash           = 92,  /* \ */
        RightBracket        = 93,  /* ] */
        GraveAccent         = 96,  /* ` */
        World1              = 161, /* non-US #
        World2              = 162, /* non-US #
        /* Function keys */
        Escape              = 256,
        Enter               = 257,
        Tab                 = 258,
        Backspace           = 259,
        Insert              = 260,
        Delete              = 261,
        Right               = 262,
        Left                = 263,
        Down                = 264,
        Up                  = 265,
        PageUp              = 266,
        PageDown            = 267,
        Home                = 268,
        End                 = 269,
        CapsLock            = 280,
        ScrollLock          = 281,
        NumLock             = 282,
        PrintScreen         = 283,
        Pause               = 284,
        F1                  = 290,
        F2                  = 291,
        F3                  = 292,
        F4                  = 293,
        F5                  = 294,
        F6                  = 295,
        F7                  = 296,
        F8                  = 297,
        F9                  = 298,
        F10                 = 299,
        F11                 = 300,
        F12                 = 301,
        F13                 = 302,
        F14                 = 303,
        F15                 = 304,
        F16                 = 305,
        F17                 = 306,
        F18                 = 307,
        F19                 = 308,
        F20                 = 309,
        F21                 = 310,
        F22                 = 311,
        F23                 = 312,
        F24                 = 313,
        F25                 = 314,
        /* Keypad */
        KP0                 = 320,
        KP1                 = 321,
        KP2                 = 322,
        KP3                 = 323,
        KP4                 = 324,
        KP5                 = 325,
        KP6                 = 326,
        KP7                 = 327,
        KP8                 = 328,
        KP9                 = 329,
        KPDecimal           = 330,
        KPDivide            = 331,
        KPMultiply          = 332,
        KPSubtract          = 333,
        KPAdd               = 334,
        KPEnter             = 335,
        KPEqual             = 336,
        LeftShift           = 340,
        LeftControl         = 341,
        LeftAlt             = 342,
        LeftSuper           = 343,
        RightShift          = 344,
        RightControl        = 345,
        RightAlt            = 346,
        RightSuper          = 347,
        Menu                = 348
    };
} // namespace Leaper



/*Unknown Key*/
#define LP_KEY_UNKNOWN            -1

/* Printable keys */
#define LP_KEY_SPACE              32
#define LP_KEY_APOSTROPHE         39  /* ' */
#define LP_KEY_COMMA              44  /* , */
#define LP_KEY_MINUS              45  /* - */
#define LP_KEY_PERIOD             46  /* . */
#define LP_KEY_SLASH              47  /* / */
#define LP_KEY_0                  48
#define LP_KEY_1                  49
#define LP_KEY_2                  50
#define LP_KEY_3                  51
#define LP_KEY_4                  52
#define LP_KEY_5                  53
#define LP_KEY_6                  54
#define LP_KEY_7                  55
#define LP_KEY_8                  56
#define LP_KEY_9                  57
#define LP_KEY_SEMICOLON          59  /* ; */
#define LP_KEY_EQUAL              61  /* = */
#define LP_KEY_A                  65
#define LP_KEY_B                  66
#define LP_KEY_C                  67
#define LP_KEY_D                  68
#define LP_KEY_E                  69
#define LP_KEY_F                  70
#define LP_KEY_G                  71
#define LP_KEY_H                  72
#define LP_KEY_I                  73
#define LP_KEY_J                  74
#define LP_KEY_K                  75
#define LP_KEY_L                  76
#define LP_KEY_M                  77
#define LP_KEY_N                  78
#define LP_KEY_O                  79
#define LP_KEY_P                  80
#define LP_KEY_Q                  81
#define LP_KEY_R                  82
#define LP_KEY_S                  83
#define LP_KEY_T                  84
#define LP_KEY_U                  85
#define LP_KEY_V                  86
#define LP_KEY_W                  87
#define LP_KEY_X                  88
#define LP_KEY_Y                  89
#define LP_KEY_Z                  90
#define LP_KEY_LEFT_BRACKET       91  /* [ */
#define LP_KEY_BACKSLASH          92  /* \ */
#define LP_KEY_RIGHT_BRACKET      93  /* ] */
#define LP_KEY_GRAVE_ACCENT       96  /* ` */
#define LP_KEY_WORLD_1            161 /* non-US #1 */
#define LP_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define LP_KEY_ESCAPE             256
#define LP_KEY_ENTER              257
#define LP_KEY_TAB                258
#define LP_KEY_BACKSPACE          259
#define LP_KEY_INSERT             260
#define LP_KEY_DELETE             261
#define LP_KEY_RIGHT              262
#define LP_KEY_LEFT               263
#define LP_KEY_DOWN               264
#define LP_KEY_UP                 265
#define LP_KEY_PAGE_UP            266
#define LP_KEY_PAGE_DOWN          267
#define LP_KEY_HOME               268
#define LP_KEY_END                269
#define LP_KEY_CAPS_LOCK          280
#define LP_KEY_SCROLL_LOCK        281
#define LP_KEY_NUM_LOCK           282
#define LP_KEY_PRINT_SCREEN       283
#define LP_KEY_PAUSE              284
#define LP_KEY_F1                 290
#define LP_KEY_F2                 291
#define LP_KEY_F3                 292
#define LP_KEY_F4                 293
#define LP_KEY_F5                 294
#define LP_KEY_F6                 295
#define LP_KEY_F7                 296
#define LP_KEY_F8                 297
#define LP_KEY_F9                 298
#define LP_KEY_F10                299
#define LP_KEY_F11                300
#define LP_KEY_F12                301
#define LP_KEY_F13                302
#define LP_KEY_F14                303
#define LP_KEY_F15                304
#define LP_KEY_F16                305
#define LP_KEY_F17                306
#define LP_KEY_F18                307
#define LP_KEY_F19                308
#define LP_KEY_F20                309
#define LP_KEY_F21                310
#define LP_KEY_F22                311
#define LP_KEY_F23                312
#define LP_KEY_F24                313
#define LP_KEY_F25                314
#define LP_KEY_KP_0               320
#define LP_KEY_KP_1               321
#define LP_KEY_KP_2               322
#define LP_KEY_KP_3               323
#define LP_KEY_KP_4               324
#define LP_KEY_KP_5               325
#define LP_KEY_KP_6               326
#define LP_KEY_KP_7               327
#define LP_KEY_KP_8               328
#define LP_KEY_KP_9               329
#define LP_KEY_KP_DECIMAL         330
#define LP_KEY_KP_DIVIDE          331
#define LP_KEY_KP_MULTIPLY        332
#define LP_KEY_KP_SUBTRACT        333
#define LP_KEY_KP_ADD             334
#define LP_KEY_KP_ENTER           335
#define LP_KEY_KP_EQUAL           336
#define LP_KEY_LEFT_SHIFT         340
#define LP_KEY_LEFT_CONTROL       341
#define LP_KEY_LEFT_ALT           342
#define LP_KEY_LEFT_SUPER         343
#define LP_KEY_RIGHT_SHIFT        344
#define LP_KEY_RIGHT_CONTROL      345
#define LP_KEY_RIGHT_ALT          346
#define LP_KEY_RIGHT_SUPER        347
#define LP_KEY_MENU               348

#define LP_KEY_LAST               LP_KEY_MENU

//Mouse Button
#define LP_MOUSE_BUTTON_LEFT      1
#define LP_MOUSE_BUTTON_RIGHT     2