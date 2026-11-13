#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <glm/glm.hpp>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.14159265358979323846f

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

#define RADIANS(deg) deg * (PI/180.0f)
#define DEGREES(radians) (radians * 180.0f) / PI

#define ERROR(msg) {fprintf(stderr, "[ERROR] %s\n", msg); glfwTerminate(); exit(1);}

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1f
#define SCROLL_SENSITIVITY 0.15f

#define BALL_Z_VALUE 0.8965f
#define CUEBALL_INITIAL_POS vec3(0.0f, 0.0f, 0.8965f)

#define CUE_INITIAL_DISTANCE 0.8f
#define CUE_MIN_DISTANCE 0.7407f
#define CUE_MAX_DISTANCE 1.0f + CUE_INITIAL_DISTANCE
#define CUE_MAX_PHI 87.5f
#define CUE_INITIAL_PHI CUE_MAX_PHI
#define CUE_MIN_PHI 80.0f
#define CUE_INITIAL_THETA 0.0f
#define CUE_BACKWARDS_SPEED 1.0f
#define CUE_FORWARDS_SPEED -10.f // Negative because the distance is descreasing 

#define CAM_Z_OFFSET 0.06f
#define CAM_INITIAL_ANCHOR vec3(0.0f, 0.0f, BALL_Z_VALUE + CAM_Z_OFFSET)
#define CAM_FOV 45.0f
#define CAM_ASPECT_RATIO (float) WINDOW_WIDTH/WINDOW_HEIGHT
#define CAM_NEAR 0.1f
#define CAM_FAR 100.0f
#define CAM_INITIAL_THETA CUE_INITIAL_THETA
#define CAM_INITIAL_PHI CUE_MAX_PHI
#define CAM_INITIAL_DISTANCE 1.0f
#define CAM_MIN_DISTANCE 0.15f
#define CAM_MAX_DISTANCE 1.7f

#define CAM_CANNOT_MOVE state == Game_State::TAKING_SHOT

#define PRESSING_W (key == GLFW_KEY_W && action == GLFW_PRESS)
#define PRESSING_A (key == GLFW_KEY_A && action == GLFW_PRESS)
#define PRESSING_S (key == GLFW_KEY_S && action == GLFW_PRESS)
#define PRESSING_D (key == GLFW_KEY_D && action == GLFW_PRESS)

#define PRESSING_ESC (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)

#define PRESSING_SPACE (key == GLFW_KEY_SPACE && action == GLFW_PRESS)

#define RELEASING_W (key == GLFW_KEY_W && action == GLFW_RELEASE)
#define RELEASING_A (key == GLFW_KEY_A && action == GLFW_RELEASE)
#define RELEASING_S (key == GLFW_KEY_S && action == GLFW_RELEASE)
#define RELEASING_D (key == GLFW_KEY_D && action == GLFW_RELEASE)

#define RELEASING_ESC (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)

#define RELEASING_SPACE (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)

/* #define PRESSING_W (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
#define PRESSING_A (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
#define PRESSING_S (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
#define PRESSING_D (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)

#define RELEASING_W (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
#define RELEASING_A (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
#define RELEASING_S (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
#define RELEASING_D (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) */

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::quat;

typedef uint8_t byte;