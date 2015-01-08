#pragma once

#include <string>
#include "Model.h"
using namespace Metabot;

void saveModelToFileAscii(const char *filename, Model *model);
Model loadModelSTL_ascii(const char* filename);
void saveModelToFileBinary(const char *filename, Model *model);
Model loadModelSTL_binary(const char* filename);
Model loadModelSTL(const char* filename);
Model loadModelFromFile(const char* filename);
Model loadModelSTL_string(std::string str);
