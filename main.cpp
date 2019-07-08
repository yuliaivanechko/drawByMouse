#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include <iostream>

#define CHECK( EXPR... )                                                                                                    \
glGetError();                                                                                                           \
EXPR;                                                                                                                   \
if (auto errorCode = glGetError(); GL_NO_ERROR != errorCode) {                                                          \
std::cerr << "Got error: 0x" << std::hex << errorCode << " at " << __FILE__ << ":" << std::dec << __LINE__ << "\n";     \
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse(GLFWwindow* w, int button, int action, int mode);
void drawCircle (GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int points = 0;
int countX = 0, cX = 0, cirX = 0;
int countY = 1, cY = 1, cirY = 1;
int triangle = 0;
int line = 1;
int circle = 0;
float zoomX = 1.0, zoomY = 1.0;
float transX = 0.0;
float transY = 0.0;
int point = 0;
int indiceCount = 0;
unsigned int countLines = 0;
unsigned int countRectangles = 0;
unsigned int countCircles = 0;


double aspectRatio = (double)SCR_WIDTH / (double)SCR_HEIGHT;

float lines[408] = {
    // positions         // colors
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p1
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p2
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p3
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p4
};
float rectangles[432] = {
    // positions         // colors
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p1
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p2
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p3
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // p4
};
float circles[432] = {};

unsigned int indices1[108] = {
    0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7, 8, 9, 11, 9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23, 24, 25, 27, 25, 26, 27, 28, 29, 31, 29, 30, 31, 32, 33, 35, 33, 34, 35, 36, 37, 39, 37, 38, 39, 40, 41, 43, 41, 42, 43, 44, 45, 47, 45, 46, 47, 48, 49, 51, 49, 50, 51, 52, 53, 55, 53, 54, 55, 56, 57, 59, 57, 58, 59, 60, 61, 63, 61, 62, 63, 64, 65, 67, 65, 66, 67, 68, 69, 71, 69, 70, 71,
};

float r = 1.0f, g = 1.0f, b = 1.0f;
GLint aColor, aPos;
float NorX1, NorY1, NorX2, NorY2;
int drawFlag = 0;
int red = 3, green = 4, blue = 5;
int rd = 3, grn = 4, bl = 5;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader ourShader("/Developer/Task Quad/3.3.shader.vs","/Developer/Task Quad/3.3.shader.fs");
    Shader matrixShader ("/Developer/Task Quad/matrix.vs","/Developer/Task Quad/matrix.fs");
    Shader circleShader ("/Developer/Task Quad/circle.vs","/Developer/Task Quad/circle.fs");

    aColor = glGetUniformLocation(ourShader.ID, "aColor");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    GLuint VBO, VAOs[3], EBO;
    glGenVertexArrays(3, VAOs);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer (GL_ARRAY_BUFFER, EBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    //line setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
    //vertex atttibute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //rectangle setup
    CHECK(glBindVertexArray(VAOs[1]););
    CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO););
    CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(rectangles), rectangles, GL_STATIC_DRAW););
    //vertex atttibute
    CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0););
    CHECK(glEnableVertexAttribArray(0););
    // color attribute
    CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))););
    CHECK(glEnableVertexAttribArray(1););
    
    CHECK(glBindVertexArray(VAOs[2]););
    CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(circles), circles, GL_STATIC_DRAW););
    //vertex atttibute
    CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0););
    CHECK(glEnableVertexAttribArray(0););
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        CHECK(glBindVertexArray(VAOs[0]););
        
        if (drawFlag == 1)
        {
            if(line == 1)
            {
            glUseProgram(ourShader.ID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
            glUniform3f(aColor, r, g, b);
            processInput(window);
            lines[red] = r; red+=6;
            lines[green] = g; green+=6;
            lines[blue] = b; blue+=6;
            lines[red] = r; red+=6;
            lines[green] = g; green+=6;
            lines[blue] = b; blue+=6;
            point = 2;
            }
            else if (triangle==1)
            {
                glUseProgram(ourShader.ID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(rectangles), rectangles, GL_STATIC_DRAW);
                processInput(window);
                rectangles[rd] = r; rd+=6;
                rectangles[grn] = g; grn+=6;
                rectangles[bl] = b; bl+=6;
                rectangles[rd] = r; rd+=6;
                rectangles[grn] = g; grn+=6;
                rectangles[bl] = b; bl+=6;
                rectangles[rd] = r; rd+=6;
                rectangles[grn] = g; grn+=6;
                rectangles[bl] = b; bl+=6;
                rectangles[rd] = r; rd+=6;
                rectangles[grn] = g; grn+=6;
                rectangles[bl] = b; bl+=6;
                points = 4;
            }
            else if (circle == 1)
            {
                glUseProgram(circleShader.ID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(circles), circles, GL_STATIC_DRAW);
                processInput(window);
            }
            drawFlag = 0;
        }

        glUseProgram(matrixShader.ID);
        glm::mat4 view;
        view = glm::translate(view, glm::vec3(transX, transY, 0.0));
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(zoomX,zoomY,0.0));
        matrixShader.setMat4("model", model);
        matrixShader.setMat4("view", view);
        CHECK(glBindVertexArray(VAOs[0]););
        CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW););
        point = 68;
        CHECK(glDrawArrays(GL_LINES, 0, point););
        glBindVertexArray(VAOs[1]);
        CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(rectangles), rectangles, GL_STATIC_DRAW););
        points = 108;
        CHECK(glDrawElements(GL_TRIANGLES, points, GL_UNSIGNED_INT, 0););
      
        
        glUseProgram(circleShader.ID);

        
        glm::mat4 viewC;
        viewC = glm::translate(viewC, glm::vec3(transX, transY, 0.0));
       
        glm::mat4 modelC;
        modelC = glm::scale(modelC, glm::vec3(zoomX,zoomY,0.0));
        circleShader.setMat4("modelC", modelC);
        circleShader.setMat4("viewC", viewC);
        
        glBindVertexArray(VAOs[2]);
        CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(circles), circles, GL_STATIC_DRAW););
        points = 72;
        for (int i = 0; i< 24; i++)
        {
        glUniform2f(glGetUniformLocation(circleShader.ID, "pos1"), circles[i*18], circles[i*18+1]);
        glUniform2f(glGetUniformLocation(circleShader.ID, "pos2"), circles[i*18+6], circles[i*18+7]);
        float radius = (sqrt(pow((circles[(i*18)+3] - circles[i*18]),2) + pow((circles[(i*18)+4] - circles[i*18+1]),2)))/2;
            glm::vec4 rad(radius, 0.0, 0.0, 1.0);
            glm::mat4 zoomR;
            zoomR = glm::scale(zoomR, glm::vec3(zoomX, zoomY, 0.0));
            rad = zoomR * rad;
        circleShader.setFloat("radius", rad.x);
        CHECK(glDrawArrays(GL_TRIANGLES, i*6, 6););
        }

        
     
        glfwSwapBuffers(window);
        glfwPollEvents();
        glBindVertexArray(0);
    }
    glfwTerminate();
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {r = 1.0; g = 0.0; b = 0.0; }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    { r = 0.0; g = 1.0; b = 0.0; }
    else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    { r = 0.0; g = 0.0; b = 1.0; }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    { r = 1.0; g = 1.0; b = 1.0; }
    else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    { r = 1.0; g = 1.0; b = 0.0; }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    { r = 1.0; g = 0.5; b = 0.0; }
    else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    { r = 0.8; g = 0.196; b = 0.6; }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    { r = 0.858824; g = 0.439216; b = 0.858824; }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    { r = 0.560784; g = 0.560784; b = 0.737255; }
    else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    { r = 0.556863; g = 0.137255;  b = 0.419608; }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    { r = 0.3; g = 0.3;  b = 0.1; }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    { r = 0.96; g = 0.8; b = 0.69; }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    { r = 0.52;  g = 0.39; b = 0.39; }
    else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    { r = 0.137255; g = 0.137255; b = 0.556863; }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    { r = 0.5; g = 1.0; b = 0.0; }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    { r = 0.439216; g = 0.858824; b = 0.576471;}
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i=0, j=0; i<432; i++,j++)
        {
            lines[i] = 0;
            rectangles[j] = 0;
            circles[i] = 0;
        }
        countX = 0;
        countY = 1;
        drawFlag = 0;
        red = 3; green = 4; blue = 5;
        countLines = 0;
        countRectangles = 0;
        cX = 0; cY = 1;
        rd = 3; grn = 4; bl = 5;
        cirX = 0; cirY = 1;
        countCircles = 0;

    }
    else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
    {
        transX+=0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    {
        transX-=0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
    {
        transY+=0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
    {
        transY-=0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        zoomX+=0.001;
        zoomY+=0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        zoomX-=0.001;
        zoomY-=0.001;
        if(zoomX <= 0)
        {
            zoomX = 0.001;
            zoomY = 0.001;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle = 1;
        line = 0;
        circle = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle = 0;
        line = 1;
        circle = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        circle = 1;
        line = 0;
        triangle = 0;
    }
}
void mouse(GLFWwindow* w, int button, int action, int mode)
{
    double xpos = 0, ypos = 0;
    int width, height;
    //getting cursor position
    glfwGetCursorPos(w, &xpos, &ypos);
    glfwGetWindowSize(w, &width, &height);
    if (action == 1)
    {
        NorX1 = (xpos - (width / 2)) / (width / 2);
        NorY1 = -(ypos - (height / 2)) / (height / 2);
        printf("Button = %i \t isClicked? = %i \t X = %.3f \t Y = %.3f \t NorX = %.3f \t NorY = %.3f \n", button, action, xpos, ypos, NorX1, NorY1);
    }
    else if (action == 0)
    {
        drawFlag = 1;
        NorX2 = (xpos - (width / 2)) / (width / 2);
        NorY2 = -(ypos - (height / 2)) / (height / 2);
        printf("Button = %i \t isClicked? = %i \t X = %.3f \t Y = %.3f \t NorX = %.3f \t NorY = %.3f \n", button, action, xpos, ypos, NorX2, NorY2);
        if(line == 1)
        {
            glm::vec4 pos1(NorX1, NorY1, 0.0f, 1.0f);
            glm::vec4 pos2(NorX2, NorY2, 0.0f, 1.0f);
            glm::mat4 transl;
            transl = glm::translate(transl, glm::vec3(-transX,-transY,0.0));
            pos1 =  transl * pos1;
            pos2 =  transl * pos2;
            glm::mat4 scal;
            float zX = 1.0 / zoomX;
            float zY = 1.0 / zoomY;
            scal = glm::scale(scal,glm::vec3(zX,zY,0.0));
            pos1 = scal * pos1;
            pos2 = scal * pos2;
            lines[countX] = pos1.x; countX+=6;
            lines[countY] = pos1.y; countY+=6;
            lines[countX] = pos2.x; countX+=6;
            lines[countY] = pos2.y; countY+=6;
            countLines++;
            std::cout<<countLines<<" lines are drawn."<<std::endl;
        }
        else if (triangle == 1)
        {
            if(countRectangles == 18)
            {
                std::cout<<"Press BACKSPACE to draw more!"<<std::endl;
                return;
            }
            glm::vec4 pos1(NorX2, NorY1, 0.0f, 1.0f);
            glm::vec4 pos2(NorX2, NorY2, 0.0f, 1.0f);
            glm::vec4 pos3(NorX1, NorY2, 0.0f, 1.0f);
            glm::vec4 pos4(NorX1, NorY1, 0.0f, 1.0f);
            glm::mat4 transl;
            transl = glm::translate(transl, glm::vec3(-transX,-transY,0.0));
            pos1 =  transl * pos1;
            pos2 =  transl * pos2;
            pos3 =  transl * pos3;
            pos4 =  transl * pos4;
            glm::mat4 scal;
            float zX = 1.0 / zoomX;
            float zY = 1.0 / zoomY;
            scal = glm::scale(scal,glm::vec3(zX,zY,0.0));
            pos1 = scal * pos1;
            pos2 = scal * pos2;
            pos3 = scal * pos3;
            pos4 = scal * pos4;
            rectangles[cX] = pos1.x; cX+=6;
            rectangles[cY] = pos1.y; cY+=6;
            rectangles[cX] = pos2.x; cX+=6;
            rectangles[cY] = pos2.y; cY+=6;
            rectangles[cX] = pos3.x; cX+=6;
            rectangles[cY] = pos3.y; cY+=6;
            rectangles[cX] = pos4.x; cX+=6;
            rectangles[cY] = pos4.y; cY+=6;
            countRectangles++;
            std::cout<<countRectangles<<" rectangles are drawn."<<std::endl;
            }
        
        else if(circle == 1)
        {
            glm::vec4 pos1(NorX1-0.2/aspectRatio, NorY1+0.2, 0.0f, 1.0f);
            glm::vec4 pos2(NorX1+0.2/aspectRatio, NorY1+0.2, 0.0f, 1.0f);
            glm::vec4 pos3(NorX1+0.2/aspectRatio, NorY1-0.2, 0.0f, 1.0f);
            glm::vec4 pos4(NorX1+0.2/aspectRatio, NorY1-0.2, 0.0f, 1.0f);
            glm::vec4 pos5(NorX1-0.2/aspectRatio, NorY1-0.2, 0.0f, 1.0f);
            glm::vec4 pos6(NorX1-0.2/aspectRatio, NorY1+0.2, 0.0f, 1.0f);
            glm::mat4 transl;
            transl = glm::translate(transl, glm::vec3(-transX,-transY,0.0));
            pos1 =  transl * pos1;
            pos2 =  transl * pos2;
            pos3 =  transl * pos3;
            pos4 =  transl * pos4;
            pos5 =  transl * pos5;
            pos6 =  transl * pos6;
            glm::mat4 scal;
            float zX = 1.0 / zoomX;
            float zY = 1.0 / zoomY;
            scal = glm::scale(scal,glm::vec3(zX,zY,0.0));
            pos1 = scal * pos1;
            pos2 = scal * pos2;
            pos3 = scal * pos3;
            pos4 = scal * pos4;
            pos5 = scal * pos5;
            pos6 = scal * pos6;
            circles[cirX] = pos1.x; cirX+=3;
            circles[cirY] = pos1.y; cirY+=3;
            circles[cirX] = pos2.x; cirX+=3;
            circles[cirY] = pos2.y; cirY+=3;
            circles[cirX] = pos3.x; cirX+=3;
            circles[cirY] = pos3.y; cirY+=3;
            circles[cirX] = pos4.x; cirX+=3;
            circles[cirY] = pos4.y; cirY+=3;
            circles[cirX] = pos5.x; cirX+=3;
            circles[cirY] = pos5.y; cirY+=3;
            circles[cirX] = pos6.x; cirX+=3;
            circles[cirY] = pos6.y; cirY+=3;
            
            countCircles++;
            std::cout<<countCircles<<" circles are drawn."<<std::endl;
        }
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


