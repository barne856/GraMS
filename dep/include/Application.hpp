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

#ifndef __APPLICATION__
#define __APPLICATION__
#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "vmath.h"

class Application
{
private:
    struct APPINFO
    {
        unsigned int windowWidth;
        unsigned int windowHeight;
        std::string Title;
        short samples;
    };
    
protected:
    APPINFO info;
    GLFWwindow* window;
public:
    Application() {}
    ~Application() {}
    virtual void Init()
    {
        info.windowWidth = 720;
        info.windowHeight = 720;
        info.Title = "Untitled";
        info.samples = 1;
    }
    virtual void Startup() {}
    virtual void Render() {}
    virtual void Shutdown() {}
    void Run()
    {
        Init();

        if(!glfwInit())
        {
            std::cout << "GLFW Failed to Initialize!" << std::endl;
        }

        glfwWindowHint(GLFW_SAMPLES, info.samples);

        window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.Title.c_str(), NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to Create Window!" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);

        if(glewInit() != GLEW_OK)
        {
            std::cout << "GLEW Failed to Initialize!" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "      ___          ___          ___          ___          ___     " << std::endl;
        std::cout << "     /\\  \\        /\\  \\        /\\  \\        /\\__\\        /\\  \\    " << std::endl;
        std::cout << "    /::\\  \\      /::\\  \\      /::\\  \\      /::|  |      /::\\  \\   " << std::endl;
        std::cout << "   /:/\\:\\  \\    /:/\\:\\  \\    /:/\\:\\  \\    /:|:|  |     /:/\\ \\  \\  " << std::endl;
        std::cout << "  /:/  \\:\\  \\  /::\\~\\:\\  \\  /::\\~\\:\\  \\  /:/|:|__|__  _\\:\\~\\ \\  \\ " << std::endl;
        std::cout << " /:/__/_\\:\\__\\/:/\\:\\ \\:\\__\\/:/\\:\\ \\:\\__\\/:/ |::::\\__\\/\\ \\:\\ \\ \\__\\" << std::endl;
        std::cout << " \\:\\  /\\ \\/__/\\/_|::\\/:/  /\\/__\\:\\/:/  /\\/__/~~/:/  /\\:\\ \\:\\ \\/__/" << std::endl;
        std::cout << "  \\:\\ \\:\\__\\     |:|::/  /      \\::/  /       /:/  /  \\:\\ \\:\\__\\  " << std::endl;
        std::cout << "   \\:\\/:/  /     |:|\\/__/       /:/  /       /:/  /    \\:\\/:/  /  " << std::endl;
        std::cout << "    \\::/  /      |:|  |        /:/  /       /:/  /      \\::/  /   " << std::endl;
        std::cout << "     \\/__/        \\|__|        \\/__/        \\/__/        \\/__/    " << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "                  Granular Material Simulator" << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
        std::cout << "OpenGL Verison " << glGetString(GL_VERSION) << std::endl;
        
        Startup();
        while (!glfwWindowShouldClose(window))
        {
            Render();

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        Shutdown();

    glfwTerminate();
    }

};

#endif//__APPLICATION__