#pragma once

#include <string>
#include <vector>

#include "defs.hpp"
#include "fdata.hpp"

namespace ftemplates{
    void create_project(const std::string& name, project::type t, project::build b, const char* project_file, const char* templates_file, const char* modules_file);

    void update_run_script(const std::string& p, project::build b, bool fast, unsigned int enabled = 0);
    void update_make_script(const std::string& p, const fdata::ProjectModulesFile& modules, project::build b);

    void update_templates_file(const std::string& p, const std::vector<fdata::ProjectTemplate>& templates);

    void build_template(const fdata::ProjectCompiledTemplate& t);
}