#pragma once

#include <string>
#include <vector>

#include "defs.hpp"
#include "fdata.hpp"

namespace ftemplates{
    void create_project(const std::string& name, project::type t, project::build b);

    void update_run_script(const std::string& p, project::build b, bool fast, unsigned int enabled = 0);
    void update_make_script(const std::string& p, const std::string& modules);

    void update_templates_file(const std::string& p, const std::vector<fdata::ProjectTemplate>& templates);
    void update_modules_file(const std::string& p, const fdata::ProjectTemplate& tmpl);

    void build_template(const fdata::ProjectCompiledTemplate& t);
}