#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

#include <iostream>

// Game
#include "Block.h"
#include "Chunk.h"
#include "ChunkRenderer.h"
#include "BlockRenderer.h"
#include "structures.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "World.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//Camera camera(glm::vec3(8.0f, 8.0f, 20.0f));


// Create a projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

// Caemra
Camera camera(glm::vec3(0.3f, 0.5f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


float _SPEED_UP = 3.0f;


void printMat4(const glm::mat4& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


float big_position = 0.5f;
// lighting
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader basicShader("C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\basicShader.vs", "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\basicShader.fs");
    Shader textureShader("C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\textureShader.vs", "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\textureShader.fs");


    // Create chunk
    /*Chunk chunk = Chunk();
       
    ChunkRenderer chunkRenderer(chunk);*/

    //Block block(glm::vec3(0.0f, 0.0f, 0.0f), BlockType::AIR);
    //BlockRenderer blockRenderer(block, basicShader);

    BlockLighting blockLighting;
    blockLighting.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    blockLighting.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    blockLighting.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    blockLighting.shininess = 32.0f;

    // render loop

    World world(1, 1);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // rendering commands here
        glClearColor(1.0f, 0.5f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);

        lightPos = camera.Position;
        view = camera.GetViewMatrix();
      

        // 
        // 
        // world transf
        glm::mat4 model = glm::mat4(1.0f);
        basicShader.setMat4("model", model);
        basicShader.setMat4("view", view);
        basicShader.setVec3("lightPos", lightPos);
        basicShader.setMat4("projection", projection);
        basicShader.setVec3("viewPos", camera.Position);

        textureShader.setMat4("model", model);
        textureShader.setMat4("view", view);
        textureShader.setVec3("lightPos", lightPos);
        textureShader.setMat4("projection", projection);
        textureShader.setVec3("viewPos", camera.Position);

        ////// Render the chunk

        world.draw(textureShader);

        //chunkRenderer.render(textureShader);
        //chunkRenderer.renderBlock(chunk.getBlock(0, 0, 0));
   
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime*_SPEED_UP);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime* _SPEED_UP);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime*_SPEED_UP);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime*_SPEED_UP);

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on Retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}