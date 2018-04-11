#pragma once

#include "Shader.h"

extern  GLuint loadTexture(std::string fileName, GLint REPEAT, GLint FILTER);

void DrawFloor(Shader *shader);
