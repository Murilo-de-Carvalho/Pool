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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.14159265358979323846f

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

#define RADIANS(deg) deg * (PI/180.0f)
#define DEGREES(radians) (radians * 180.0f) / PI

#define ERROR(msg) {fprintf(stderr, "[ERROR] %s\n", msg); glfwTerminate(); exit(1);}

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::quat;

typedef uint8_t byte;