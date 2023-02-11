#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <string>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

#include "defaults.hpp"
#include "dog.hpp"


namespace
{
    constexpr char const* kWindowTitle = "COMP3811 - Coursework 2";

    void glfw_callback_error_(int, char const*);

    void glfw_callback_key_(GLFWwindow*, int, int, int, int);
    void glfw_callback_motion_(GLFWwindow*, double, double);
    void cameraControl(GLFWwindow*);
    void animationControl(GLFWwindow*);
    // draw objects with SimpleMeshData structure
    void drawMesh(GLuint meshVAO, GLsizei vertexCount, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    // draw point light source
    void drawLight(GLuint meshVAO, GLsizei vertexCount, glm::mat4 view, glm::mat4 projection, glm::vec3 pointLightPositions[]);
    // set light model parameters
    void setLightModel(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, glm::vec3 pointLightPositions[], GLuint programID, bool multiTexture);

    struct GLFWCleanupHelper
    {
        ~GLFWCleanupHelper();
    };
    struct GLFWWindowDeleter
    {
        ~GLFWWindowDeleter();
        GLFWwindow* window;
    };
}

// camera parameters
glm::vec3 positionCam = glm::vec3(0.f, 0.f, 5.f);
glm::vec3 frontCam = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 upCam = glm::vec3(0.f, 1.f, 0.f);
bool activeCam = false;

bool initialMouse = true;
float angleYaw = -90.f;
float anglePitch = 0.f;
float lastX = 1280.f / 2.f;
float lastY = 720.f / 2.f;
float viewField = 45.f;

// timing
float timeChange = 0.0f;
float lastFrame = 0.0f;
float speedMulitiplier = 1.f;

// animation
bool animationActive = true;
float animatedLightPosx;
float animatedLightPosz;
float animatedLightAngle;
float staticLightPosAngle;
float pauseTime = 0.f;
float animationSpeedMultiplier = 0.5f;

// different light model
bool blinn = false;
bool blinnModelPressed = false;


int main() try
{
    // Initialize GLFW
    if (GLFW_TRUE != glfwInit())
    {
        char const* msg = nullptr;
        int ecode = glfwGetError(&msg);
        throw Error("glfwInit() failed with '%s' (%d)", msg, ecode);
    }

    // Ensure that we call glfwTerminate() at the end of the program.
    GLFWCleanupHelper cleanupHelper;

    // Configure GLFW and create window
    glfwSetErrorCallback(&glfw_callback_error_);

    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    //glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);

#	if !defined(NDEBUG)
    // When building in debug mode, request an OpenGL debug context. This
    // enables additional debugging features. However, this can carry extra
    // overheads. We therefore do not do this for release builds.
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#	endif // ~ !NDEBUG

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        kWindowTitle,
        nullptr, nullptr
    );

    if (!window)
    {
        char const* msg = nullptr;
        int ecode = glfwGetError(&msg);
        throw Error("glfwCreateWindow() failed with '%s' (%d)", msg, ecode);
    }

    GLFWWindowDeleter windowDeleter{ window };

    // Set up event handling
    glfwSetKeyCallback(window, &glfw_callback_key_);
    glfwSetCursorPosCallback(window, &glfw_callback_motion_);

    // Set up drawing stuff
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync is on.

    // Initialize GLAD
    // This will load the OpenGL API. We mustn't make any OpenGL calls before this!
    if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
        throw Error("gladLoaDGLLoader() failed - cannot load GL API!");

    std::printf("RENDERER %s\n", glGetString(GL_RENDERER));
    std::printf("VENDOR %s\n", glGetString(GL_VENDOR));
    std::printf("VERSION %s\n", glGetString(GL_VERSION));
    std::printf("SHADING_LANGUAGE_VERSION %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Ddebug output
#	if !defined(NDEBUG)
    setup_gl_debug_output();
#	endif // ~ !NDEBUG

    // Global GL state
    OGL_CHECKPOINT_ALWAYS();

    // TODO: global GL setup goes here
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    OGL_CHECKPOINT_ALWAYS();

    // Get actual framebuffer size.
    // This can be different from the window size, as standard window
    // decorations (title bar, borders, ...) may be included in the window size
    // but not be part of the drawable surface area.
    int iwidth, iheight;
    glfwGetFramebufferSize(window, &iwidth, &iheight);

    glViewport(0, 0, iwidth, iheight);

    // Other initialization & loading
    OGL_CHECKPOINT_ALWAYS();

    // TODO: load shader program
    // default prograrm
    ShaderProgram prog({
       { GL_VERTEX_SHADER, "assets/default.vert" },
       { GL_FRAGMENT_SHADER, "assets/default.frag" }
        });

    // one texture program
    ShaderProgram textureProg({
       { GL_VERTEX_SHADER, "assets/texture.vert" },
       { GL_FRAGMENT_SHADER, "assets/texture.frag" }
        });

    // multi-texture program
    ShaderProgram multiTextureProg({
       { GL_VERTEX_SHADER, "assets/multiTexture.vert" },
       { GL_FRAGMENT_SHADER, "assets/multiTexture.frag" }
        });

    // light cube program
    ShaderProgram lightCube({
       { GL_VERTEX_SHADER, "assets/lightCube.vert" },
       { GL_FRAGMENT_SHADER, "assets/lightCube.frag" }
        });

    auto last = Clock::now();




    // draw objects using SimpleMeshData structure
    // light
    SimpleMeshData lights = makeCube();
    GLuint lightsSimpleMeshVAO = create_vao(lights);
    GLsizei lightsCount = static_cast<GLsizei>(lights.positions.size());

    // house
    SimpleMeshData house = makeHouse();
    GLuint houseSimpleMeshVAO = create_vao(house);
    GLsizei houseCount = static_cast<GLsizei>(house.positions.size());

    // roof with two textures
    std::string roofTexturePath = "assets/Chongshou.png";
    std::string specularPath = "assets/specularTexture.jpg";
    SimpleMeshData roof = makeRoof();
    std::vector<GLuint> roofVector = create_vao(roof, roofTexturePath, roofTexturePath);
    GLuint roofSimpleMeshVAO = roofVector.at(0);
    GLuint roofTexture = roofVector.at(1);
    GLuint roofSpecular = roofVector.at(2);
    GLsizei roofCount = static_cast<GLsizei>(roof.positions.size());

    // house - mainly diffuse material
    glm::vec3 houseDiffuse = glm::vec3{ 0.98f, 0.92f, 0.69f };
    glm::vec3 houseSpecular = glm::vec3{ 0.1f, 0.1f, 0.1f };
    glm::vec3 houseEmissive = glm::vec3{ 0.01f, 0.01f, 0.01f };

    glm::mat4 houseModel = glm::mat4(1.0f);
    houseModel = glm::scale(houseModel, glm::vec3{ 2.f });

    // bowl
    SimpleMeshData bowl = makeBowl();
    GLuint bowlSimpleMeshVAO = create_vao(bowl);
    GLsizei bowlCount = static_cast<GLsizei>(bowl.positions.size());

    // mainly specular material
    glm::vec3 bowlDiffuse = glm::vec3{ 0.5f,0.5f,0.5f };
    glm::vec3 bowlSpecular = glm::vec3{ 0.7f, 0.7f, 0.7f };
    glm::vec3 bowlEmissive = glm::vec3{ 0.01f, 0.01f, 0.01f };

    glm::mat4 bowlModel = glm::mat4(1.0f);
    bowlModel = glm::scale(bowlModel, glm::vec3{ 0.4f });
    bowlModel = glm::translate(bowlModel, glm::vec3{ 3.f, -3.f, -1.3f });

    // bed
    std::string bedTexturePath = "assets/wood.jpg";
    SimpleMeshData bed = makeBed();
    std::vector<GLuint> bedVector = create_vao(bed, bedTexturePath, "");
    GLuint bedSimpleMeshVAO = bedVector.at(0);
    GLuint bedTexture = bedVector.at(1);
    GLsizei bedCount = static_cast<GLsizei>(bed.positions.size());

    // mainly emissive material
    glm::vec3 bedDiffuse = glm::vec3{ 0.01f, 0.01f, 0.01f };
    glm::vec3 bedSpecular = glm::vec3{ 0.01f, 0.01f, 0.01f };
    glm::vec3 bedEmissive = glm::vec3{ 0.12f, 0.08f, 0.06f };

    // contrast for not mainly emissive material
    // glm::vec3 bedDiffuse = glm::vec3{0.7f, 0.7f, 0.7f};
    // glm::vec3 bedSpecular = glm::vec3{0.5f, 0.5f, 0.5f};
    // glm::vec3 bedEmissive = glm::vec3{ 0.01f, 0.01f, 0.01f };

    glm::mat4 bedModel = glm::mat4(1.0f);
    bedModel = glm::translate(bedModel, glm::vec3{ -2.f, -1.3f, -0.2f });


    // paw
    SimpleMeshData paw = makePaw();
    GLuint pawSimpleMeshVAO = create_vao(paw);
    GLsizei pawCount = static_cast<GLsizei>(paw.positions.size());

    glm::vec3 pawDiffuse = glm::vec3{ 0.5f,0.5f,0.5f };
    glm::vec3 pawSpecular = glm::vec3{ 0.1f, 0.1f, 0.1f };
    glm::vec3 pawEmissive = glm::vec3{ 0.01f, 0.01f, 0.01f };

    glm::mat4 pawModel = glm::mat4(1.0f);
    pawModel = glm::translate(pawModel, glm::vec3{ 2.5f, -1.f, 0.f });

    // ball
    SimpleMeshData ball = makeBall();
    GLuint ballSimpleMeshVAO = create_vao(ball);
    GLsizei ballCount = static_cast<GLsizei>(ball.positions.size());

    glm::vec3 ballDiffuse = glm::vec3{ 0.3f,0.3f,0.3f };
    glm::vec3 ballSpecular = glm::vec3{ 0.3f, 0.3f, 0.3f };
    glm::vec3 ballEmissive = glm::vec3{ 0.01f, 0.01f, 0.01f };

    // ball movement
    float ballSpeed = 0.2f, ballFrame = 0.f, roundTime = 4.f, baseTime = 0.f;
    float ballX = 0.f, ballZ = 0.f, xPath = 0.f, zPath = 0.f;
    int roundNum = 0;

    // set texture
    glUseProgram(textureProg.programId());
    glUniform1i(glGetUniformLocation(textureProg.programId(), "iTexture"), 0);

    // set muli-texture
    glUseProgram(multiTextureProg.programId());
    glUniform1i(glGetUniformLocation(multiTextureProg.programId(), "iTexture"), 1);
    glUniform1i(glGetUniformLocation(multiTextureProg.programId(), "material.specular"), 2);


    // reset state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // clean up buffers

    OGL_CHECKPOINT_ALWAYS();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Let GLFW process events
        glfwPollEvents();

        float currentFrame = static_cast<float>(glfwGetTime());
        timeChange = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // set shading model
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnModelPressed) {
            blinn = !blinn;
            blinnModelPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
            blinnModelPressed = false;
        }
        glUseProgram(multiTextureProg.programId());
        glUniform1i(glGetUniformLocation(multiTextureProg.programId(), "blinn"), blinn);

        roundNum = static_cast<int>(currentFrame / (4.f * roundTime));
        baseTime = roundNum * 4.f * roundTime;

        // ball movement
        // move left
        if (currentFrame > baseTime && currentFrame <= baseTime + roundTime) {
            ballFrame = currentFrame - roundNum * 4 * roundTime;
            xPath = ballX - ballSpeed * ballFrame;
            zPath = 0.5;
        }
        // move front
        else if (currentFrame > baseTime + roundTime && currentFrame <= baseTime + 2 * roundTime) {
            ballX = xPath;
            ballFrame = currentFrame - (roundNum * 4 + 1) * roundTime;
            zPath = ballSpeed * ballFrame + 0.5f;
        }
        // move right
        else if (currentFrame > baseTime + 2 * roundTime && currentFrame <= baseTime + 3 * roundTime) {
            ballZ = zPath;
            ballFrame = currentFrame - (roundNum * 4 + 2) * roundTime;
            xPath = ballX + ballSpeed * ballFrame;
        }
        // move back
        else if (currentFrame > baseTime + 3 * roundTime && currentFrame <= baseTime + 4 * roundTime) {
            ballX = xPath;
            ballFrame = currentFrame - (roundNum * 4 + 3) * roundTime;
            zPath = ballZ - ballSpeed * ballFrame;
        }

        glm::mat4 ballModel = glm::mat4(1.0f);
        ballModel = glm::translate(ballModel, glm::vec3{ xPath, -1.f, zPath });

        cameraControl(window);
        animationControl(window);
        // Check if window was resized.
        float fbwidth, fbheight;
        {
            int nwidth, nheight;
            glfwGetFramebufferSize(window, &nwidth, &nheight);

            fbwidth = float(nwidth);
            fbheight = float(nheight);

            if (0 == nwidth || 0 == nheight)
            {
                // Window minimized? Pause until it is unminimized.
                // This is a bit of a hack.
                do
                {
                    glfwWaitEvents();
                    glfwGetFramebufferSize(window, &nwidth, &nheight);
                } while (0 == nwidth || 0 == nheight);
            }

            glViewport(0, 0, nwidth, nheight);
        }

        //TODO: update state
        auto const now = Clock::now();
        last = now;

        // Draw scene
        OGL_CHECKPOINT_DEBUG();

        //TODO: draw frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // control light cube animation
        glm::vec3 lightPos;
        if (animationActive) {
            animatedLightPosx = sin(animatedLightAngle);
            animatedLightPosz = cos(animatedLightAngle);
            lightPos = glm::vec3(animatedLightPosx, 1.f, animatedLightPosz);
        }
        else {
            lightPos = glm::vec3(sin(staticLightPosAngle), 1.f, cos(staticLightPosAngle));
        }


        lightPos.x += 3.f;
        lightPos.y -= 1.3f;
        lightPos.z -= 0.5f;

        glm::vec3 leftLight = lightPos;
        leftLight.x -= 5.f;
        leftLight.z -= 0.5f;

        // point light source
        glm::vec3 pointLightPositions[] = {
            lightPos,
            glm::vec3(1.f, 1.f, -1.f),
            leftLight
        };


        glm::mat4 view = glm::lookAt(positionCam, positionCam + frontCam, upCam);
        glm::mat4 projection = glm::mat4(1.0f);
        float aspect = fbwidth / fbheight;
        if (abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0)) {
            projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        }
        // draw objects
        // default program
        glUseProgram(prog.programId());
        // draw house
        setLightModel(houseDiffuse, houseSpecular, houseEmissive, pointLightPositions, prog.programId(), false);
        drawMesh(houseSimpleMeshVAO, houseCount, houseModel, view, projection);
        // draw bowl
        setLightModel(bowlDiffuse, bowlSpecular, bowlEmissive, pointLightPositions, prog.programId(), false);
        drawMesh(bowlSimpleMeshVAO, bowlCount, bowlModel, view, projection);
        // draw paw
        setLightModel(pawDiffuse, pawSpecular, pawEmissive, pointLightPositions, prog.programId(), false);
        drawMesh(pawSimpleMeshVAO, pawCount, pawModel, view, projection);
        // draw ball
        setLightModel(ballDiffuse, ballSpecular, ballEmissive, pointLightPositions, prog.programId(), false);
        drawMesh(ballSimpleMeshVAO, ballCount, ballModel, view, projection);

        // texture program
        glUseProgram(textureProg.programId());
        // draw bed
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bedTexture);
        setLightModel(bedDiffuse, bedSpecular, bedEmissive, pointLightPositions, textureProg.programId(), false);
        drawMesh(bedSimpleMeshVAO, bedCount, bedModel, view, projection);

        // multi-texture program
        glUseProgram(multiTextureProg.programId());
        // draw roof
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, roofTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, roofSpecular);
        setLightModel(houseDiffuse, houseSpecular, houseEmissive, pointLightPositions, multiTextureProg.programId(), true);
        drawMesh(roofSimpleMeshVAO, roofCount, houseModel, view, projection);

        // light cube program
        glUseProgram(lightCube.programId());
        // draw light
        drawLight(lightsSimpleMeshVAO, lightsCount, view, projection, pointLightPositions);

        // reset state
        glBindVertexArray(0);
        glUseProgram(0);
        OGL_CHECKPOINT_DEBUG();

        // Display results
        glfwSwapBuffers(window);
    }

    // Cleanup.
    //TODO: additional cleanup
    glDeleteVertexArrays(1, &lightsSimpleMeshVAO);
    glDeleteVertexArrays(1, &houseSimpleMeshVAO);
    glDeleteVertexArrays(1, &roofSimpleMeshVAO);
    glDeleteVertexArrays(1, &bowlSimpleMeshVAO);
    glDeleteVertexArrays(1, &bedSimpleMeshVAO);
    glDeleteVertexArrays(1, &pawSimpleMeshVAO);
    glDeleteVertexArrays(1, &ballSimpleMeshVAO);
    return 0;
}
catch (std::exception const& eErr)
{
    std::fprintf(stderr, "Top-level Exception (%s):\n", typeid(eErr).name());
    std::fprintf(stderr, "%s\n", eErr.what());
    std::fprintf(stderr, "Bye.\n");
    return 1;
}


namespace
{
    void glfw_callback_error_(int aErrNum, char const* aErrDesc) {
        std::fprintf(stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum);
    }

    void glfw_callback_key_(GLFWwindow* aWindow, int aKey, int, int aAction, int) {

        if (GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction)
        {
            glfwSetWindowShouldClose(aWindow, GLFW_TRUE);
            return;
        }
        // Space toggles camera
        if (GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction) {
            activeCam = !activeCam;
        }
        // p toggles animation
        if (GLFW_KEY_P == aKey && GLFW_PRESS == aAction) {
            animationActive = !animationActive;
            if (!animationActive) {
                staticLightPosAngle = animatedLightAngle;
            }
            else {
                pauseTime = static_cast<float>(glfwGetTime());
            }
        }
    }

    void cameraControl(GLFWwindow* aWindow) {
        float cameraSpeed = static_cast<float>(2.5 * timeChange * speedMulitiplier);
        if (activeCam) {
            // speed up
            if (glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                speedMulitiplier += 0.1f;
            }
            // slow down
            if (glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                speedMulitiplier -= 0.1f;
                if (speedMulitiplier <= 0.1f) {
                    speedMulitiplier = 0.1f;
                }
            }
            // change camera position
            if (glfwGetKey(aWindow, GLFW_KEY_W) == GLFW_PRESS) {
                positionCam += cameraSpeed * frontCam;
            }
            if (glfwGetKey(aWindow, GLFW_KEY_S) == GLFW_PRESS) {
                positionCam -= cameraSpeed * frontCam;
            }
            if (glfwGetKey(aWindow, GLFW_KEY_A) == GLFW_PRESS) {
                positionCam -= glm::normalize(glm::cross(frontCam, upCam)) * cameraSpeed;
            }
            if (glfwGetKey(aWindow, GLFW_KEY_D) == GLFW_PRESS) {
                positionCam += glm::normalize(glm::cross(frontCam, upCam)) * cameraSpeed;
            }
            if (glfwGetKey(aWindow, GLFW_KEY_E) == GLFW_PRESS) {
                positionCam += cameraSpeed * upCam;
            }
            if (glfwGetKey(aWindow, GLFW_KEY_Q) == GLFW_PRESS) {
                positionCam -= cameraSpeed * upCam;
            }
        }
    }

    void animationControl(GLFWwindow* aWindow) {
        animatedLightAngle = static_cast<float>((glfwGetTime() - pauseTime) * animationSpeedMultiplier + staticLightPosAngle);
        // animation speed control
        if (glfwGetKey(aWindow, GLFW_KEY_J) == GLFW_PRESS) {
            animationSpeedMultiplier += 0.001f;
        }
        if (glfwGetKey(aWindow, GLFW_KEY_K) == GLFW_PRESS) {
            if (animationSpeedMultiplier > 0.1f) {
                animationSpeedMultiplier -= 0.0001f;
            }
        }
    }

    // reference: learnopengl camera, JoeyDeVries
    void glfw_callback_motion_(GLFWwindow* aWindow, double aX, double aY)
    {
        if (activeCam) {
            float posX = static_cast<float>(aX);
            float posY = static_cast<float>(aY);

            if (initialMouse)
            {
                lastX = posX;
                lastY = posY;
                initialMouse = false;
            }

            float offsetX = posX - lastX;
            float offsetY = lastY - posY;
            lastX = posX;
            lastY = posY;

            float perceptiveness = 0.2f;
            offsetX *= perceptiveness;
            offsetY *= perceptiveness;

            angleYaw += offsetX;
            anglePitch += offsetY;

            // set bounds
            if (anglePitch > 89.f)
                anglePitch = 89.f;
            if (anglePitch < -89.f)
                anglePitch = -89.f;

            glm::vec3 front;
            front.x = cos(glm::radians(angleYaw)) * cos(glm::radians(anglePitch));
            front.y = sin(glm::radians(anglePitch));
            front.z = sin(glm::radians(angleYaw)) * cos(glm::radians(anglePitch));
            frontCam = glm::normalize(front);
        }
    }

    void drawMesh(GLuint meshVAO, GLsizei vertexCount, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(1, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(model)));
        glUniformMatrix3fv(6, 1, GL_FALSE, glm::value_ptr(normal));
        glUniform3fv(7, 1, glm::value_ptr(positionCam));

        glBindVertexArray(meshVAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }



    void setLightModel(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, glm::vec3 pointLightPositions[], GLuint programID, bool multiTexture) {
        // point light parameters
        glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.3f);
        glm::vec3 specularColor = glm::vec3(1.f, 1.f, 1.f);

        // material
        glUniform3f(glGetUniformLocation(programID, "material.ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(programID, "material.diffuse"), diffuse.x, diffuse.y, diffuse.z);
        if (!multiTexture) {
            glUniform3f(glGetUniformLocation(programID, "material.specular"), specular.x, specular.y, specular.z);
        }
        glUniform3f(glGetUniformLocation(programID, "material.emissive"), emissive.x, emissive.y, emissive.z);

        // light
        // point light 1
        glUniform3fv(glGetUniformLocation(programID, "pointLights[0].position"), 1, glm::value_ptr(pointLightPositions[0]));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[0].color"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[0].ambient"), 1, glm::value_ptr(ambientColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[0].diffuse"), 1, glm::value_ptr(diffuseColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[0].specular"), 1, glm::value_ptr(specularColor));

        // point light 2
        glUniform3fv(glGetUniformLocation(programID, "pointLights[1].position"), 1, glm::value_ptr(pointLightPositions[1]));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[1].color"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[1].ambient"), 1, glm::value_ptr(ambientColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[1].diffuse"), 1, glm::value_ptr(diffuseColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[1].specular"), 1, glm::value_ptr(specularColor));

        // point light 3
        glUniform3fv(glGetUniformLocation(programID, "pointLights[2].position"), 1, glm::value_ptr(pointLightPositions[2]));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[2].color"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[2].ambient"), 1, glm::value_ptr(ambientColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[2].diffuse"), 1, glm::value_ptr(diffuseColor));
        glUniform3fv(glGetUniformLocation(programID, "pointLights[2].specular"), 1, glm::value_ptr(specularColor));
    }

    void drawLight(GLuint meshVAO, GLsizei vertexCount, glm::mat4 view, glm::mat4 projection, glm::vec3 pointLightPositions[]) {
        glUniformMatrix4fv(1, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(meshVAO);
        for (int i = 0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
    }
}

namespace
{
    GLFWCleanupHelper::~GLFWCleanupHelper()
    {
        glfwTerminate();
    }

    GLFWWindowDeleter::~GLFWWindowDeleter()
    {
        if (window)
            glfwDestroyWindow(window);
    }
}

