#pragma once

namespace Leaper
{
    enum RenderTaskCommand
    {
        NONE,
        SHADER_UNBIND,
        SHADER_CREATE,  // 编译着色器
        SHADER_BIND,
        // Vertex array
        VAO_CREATE,  // 创建缓冲区
        VAO_BIND,
        VAO_UNBIND,
        VAO_SET_VBO,
        VAO_SET_IBO,
        // Vertex buffer
        VBO_CREATE,
        VBO_BIND,
        VBO_UNBIND,
        VBO_SET_DATA,
        // Frame buffer
        FBO_CREATE,
        FBO_BIND,
        FBO_UNBIND,
        FBO_CREATE_TEXTURE,
        FBO_CREATE_DEPTH_TEXTURE,
        FBO_RESCALE,
        FBO_READ_PIXELS,
        FBO_CLEAR_ATTACHMENT,
        //  Input
        INPUT_KEY_DOWN,
        INPUT_GET_MOUSE_POS,
        // Render Command
        DRAW_ELEMENT,  // 绘制
        CLEAR_COLOR,
        END_FRAME,  // 帧结束

        // model
        MODEL_CREATE
    };
}