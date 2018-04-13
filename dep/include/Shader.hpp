/*
 * Copyright (c) 2018 Brendan Barnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __SHADER__
#define __SHADER__
#include "GLCall.hpp"
#include <alloca.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
private:
    struct ShaderSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };
    GLuint mID;
public:
    Shader() {}
    Shader(std::string& filepath);
    ~Shader();
    ShaderSource ParseShader(std::string& filepath);
    void CompileShader(ShaderSource shaderSource);
    const GLuint inline GetID() {return mID;}
    void Bind();
};

#endif//__SHADER__