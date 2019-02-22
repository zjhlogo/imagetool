/*!
 * \file BaseType.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/14/2016
 *
 * 
 */
#include "BaseType.h"

NS_BEGIN

const tstring EMPTY_STRING = "";
const tstring NULL_STRING = "null";

const glm::ivec2 IVEC2_ZERO = glm::ivec2(0, 0);
const glm::ivec3 IVEC3_ZERO = glm::ivec3(0, 0, 0);
const glm::ivec4 IVEC4_ZERO = glm::ivec4(0, 0, 0, 0);

const glm::vec2 VEC2_ZERO = glm::vec2(0.0f, 0.0f);
const glm::vec3 VEC3_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec4 VEC4_ZERO = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

const glm::vec2 VEC2_HALF = glm::vec2(0.5f, 0.5f);
const glm::vec3 VEC3_HALF = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec4 VEC4_HALF = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);

const glm::vec2 VEC2_ONE = glm::vec2(1.0f, 1.0f);
const glm::vec3 VEC3_ONE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec4 VEC4_ONE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 VEC3_X = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 VEC3_Y = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 VEC3_Z = glm::vec3(0.0f, 0.0f, 1.0f);

const glm::quat QUAT_ZERO = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

const glm::vec4 VEC4_WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 VEC4_BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 VEC4_GRAY = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
const glm::vec4 VEC4_RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 VEC4_GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 VEC4_BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

const glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);

NS_END
