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

#include "Application.hpp"
#include <stdlib.h>
#include <time.h> 
#include <stdio.h> 
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

cv::VideoWriter outputVideo;

class Gram : public Application
{
    GLuint modelBuffer;
    GLuint positionBuffer[2];
    GLuint velocityBuffer[2];
    GLuint scaleBuffer;
    GLuint stressBuffer;
    GLuint tbo[5];
    GLuint vao[3];
    Shader render;
    Shader compute;
    unsigned int frame;
    static const int count = 3000;
    const static int sides = 64;

    void ScreenToWorld(double* xPos, double* yPos)
    {
        *xPos = 2.0f*(*xPos/info.windowWidth - 0.5f);
        *yPos = -2.0f*(*yPos/info.windowHeight - 0.5f);
    }

    void Init()
    {
        info.windowWidth = 1000;
        info.windowHeight = 1000;
        info.Title = "GraMS";
        info.samples = 16;
    }

    void Startup()
    {
        int codec = CV_FOURCC('M', 'J', 'P', 'G');
        outputVideo.open( "video.avi", codec, 60.0f, cv::Size( info.windowWidth, info.windowHeight  ), true);
        std::string filepath = "../res/shaders/render.glsl";
        render = Shader(filepath);
        filepath = "../res/shaders/compute.glsl";
        compute = Shader(filepath);

        static const char * varying_names[] =
        {
            "newPosition",
            "newVelocity",
            "newStress"
        };
        glTransformFeedbackVaryings(compute.GetID(), 3, varying_names, GL_SEPARATE_ATTRIBS);
        glLinkProgram(compute.GetID());

        float maxR = 0.02f;
        float minR = 0.01f;
        vmath::vec2 model[sides];
        vmath::vec2 positions[count];
        vmath::vec2 velocities[count];
        float stresses[count];
        float radii[count];
        srand(time(NULL));
        for(int i = 0; i < sides; i++)
        {
            float theta = 2.0*(float)M_PI * ((float)i/(float)sides);
            model[i] = vmath::vec2(cosf(theta), sinf(theta));
        }
        for(int i = 0; i < count; i++)
        {
            vmath::vec2 r1 = vmath::vec2(((float) rand() / (RAND_MAX)), ((float) rand() / (RAND_MAX)));
            vmath::vec2 r2 = vmath::vec2(((float) rand() / (RAND_MAX)), ((float) rand() / (RAND_MAX)));
            float r3 = ((float) rand() / (RAND_MAX)) * (maxR - minR) + minR;
            //float r3 = ((float) rand() / (RAND_MAX));
            radii[i] = r3;
            positions[i] = 2.0f*(r1 - vmath::vec2(0.5f));
            velocities[i] = 2.0f*(r2 - vmath::vec2(0.5f));
            stresses[i] = 0.0f;
            //if(r3 > 0.5f)
            //{
            //    radii[i] = minR;
            //}
            //else
            //{
            //    radii[i] = maxR;
            //}
            
        }
        
        glGenVertexArrays(3, vao);
        glGenBuffers(1, &modelBuffer);
        glGenBuffers(2, positionBuffer);
        glGenBuffers(2, velocityBuffer);
        glGenBuffers(1, &scaleBuffer);
        glGenBuffers(1, &stressBuffer);

        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vmath::vec2), positions, GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vmath::vec2), velocities, GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, stressBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), stresses, GL_DYNAMIC_COPY);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, scaleBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), radii, GL_DYNAMIC_COPY);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);

        glBindVertexArray(vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[1]);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vmath::vec2), positions, GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[1]);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vmath::vec2), velocities, GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, stressBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), stresses, GL_DYNAMIC_COPY);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, scaleBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), radii, GL_DYNAMIC_COPY);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);

        glBindVertexArray(vao[2]);
        glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
        glBufferData(GL_ARRAY_BUFFER, sides * sizeof(vmath::vec2), model, GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[1]);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vmath::vec2), positions, GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        glBindBuffer(GL_ARRAY_BUFFER, scaleBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), radii, GL_DYNAMIC_COPY);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        glBindBuffer(GL_ARRAY_BUFFER, stressBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), stresses, GL_DYNAMIC_COPY);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);

        glGenTextures(3, tbo);
        glBindTexture(GL_TEXTURE_BUFFER, tbo[0]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer[0]);
        glBindTexture(GL_TEXTURE_BUFFER, tbo[1]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, velocityBuffer[0]);
        glBindTexture(GL_TEXTURE_BUFFER, tbo[2]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer[1]);
        glBindTexture(GL_TEXTURE_BUFFER, tbo[3]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, velocityBuffer[1]);
        glBindTexture(GL_TEXTURE_BUFFER, tbo[4]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, scaleBuffer);

        frame = 0;
    } 
    void Render()
    {

        unsigned int nIterations = 2;
        unsigned int iteration = 0;
        const float color[4] = {0.9f, 0.9f, 0.9f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        ScreenToWorld(&xPos, &yPos);
        
        compute.Bind();
        glUniform1i(4, 0);//radii
        glUniform1i(5, 1);//positions
        glUniform1i(6, 2);//velocities
        glUniform1i(7, count);
        glUniform2f(8, (float)xPos, (float)yPos);
        float angle = (float)glfwGetTime()/20.0f;
        glUniform1f(9, angle);
        glEnable(GL_RASTERIZER_DISCARD);
        for(unsigned int i = 0; i < nIterations; i++)
        {
            glBindVertexArray(vao[iteration % 2]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_BUFFER, tbo[4]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_BUFFER, tbo[2*(iteration % 2)]);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_BUFFER, tbo[1 + 2*(iteration % 2)]);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffer[1 - (iteration % 2)]);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffer[1 - (iteration % 2)]);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, stressBuffer);
            glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, count);
            glEndTransformFeedback();
            iteration++;
        }
        glDisable(GL_RASTERIZER_DISCARD);
        
        render.Bind();
        glBindVertexArray(vao[2]);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, sides, count);
        frame++;

        cv::Mat pixels( info.windowHeight, info.windowWidth, CV_8UC3 );
        glReadPixels(0, 0, info.windowWidth, info.windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data );
        cv::Mat cv_pixels( info.windowHeight, info.windowWidth, CV_8UC3 );
        for( int y=0; y<info.windowHeight; y++ ) for( int x=0; x<info.windowWidth; x++ ) 
        {
            cv_pixels.at<cv::Vec3b>(y,x)[2] = pixels.at<cv::Vec3b>(info.windowHeight-y-1,x)[0];
            cv_pixels.at<cv::Vec3b>(y,x)[1] = pixels.at<cv::Vec3b>(info.windowHeight-y-1,x)[1];
            cv_pixels.at<cv::Vec3b>(y,x)[0] = pixels.at<cv::Vec3b>(info.windowHeight-y-1,x)[2];
        }
        outputVideo << cv_pixels;

    }
    void Shutdown()
    {
        outputVideo.release();
    }
};

int main(void)
{
    Gram gram;
    gram.Run();
    return 0;
}