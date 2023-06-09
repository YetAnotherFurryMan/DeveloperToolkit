#pragma once

#include <log.hpp>

#define STRCMP_EQ(x, y) (strcmp(x, y) == 0)

#define CREATE_DIR(name) if(!std::filesystem::create_directory(name)) dtk::log::error("Failed to create directory.", 111); //EACCES
#define CREATE_FILE(name) { std::ofstream file(name); if(!file.good()) dtk::log::error("Failed to create file.", 111); /*EACCES*/ file.close(); }
#define CREATE_SCRIPT(name) { std::ofstream file(name); if(!file.good()) dtk::log::error("Failed to create script file.", 111); /*EACCES*/ file << "#!/bin/bash" << std::endl; file.close(); }
#define FILE_OK(var) if(!var.good()) dtk::log::error("Failed to open file.", 111); //EACCES