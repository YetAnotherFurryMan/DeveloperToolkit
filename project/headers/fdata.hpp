#pragma once

#include <string>
#include <vector>

#include "defs.hpp"

#include <ml.hpp>

namespace fdata{
    struct ProjectFile{
        std::string name;
        project::type type;
        project::build build;
        int enabled = 0;

        std::string path;

        ProjectFile(const std::string& path);
        void update();
    };

    struct ProjectTemplateFile{
        std::string name;
        std::string cnt;
    };

    struct ProjectTemplateExport{
        std::string name;
        project::export_type type;
    };

    struct ProjectCompiledTemplate{
        std::vector<std::string> dirs;
        std::vector<ProjectTemplateFile> files;
    };

    struct ProjectTemplateCleanRule{
        std::vector<std::string> dirs;
        std::vector<std::string> files;
        std::vector<std::string> exes;
    };

    struct ProjectTemplateRule{
        std::string in;
        std::string out;
        std::vector<std::string> exes;
    };

    struct ProjectModule{
        std::string name;
        std::string tmpl_name;
        std::vector<ProjectTemplateExport> exports;
    };

    struct ProjectTemplate{
        std::string name;

        std::vector<std::string> dirs;
        std::vector<ProjectTemplateFile> files;
        std::vector<ProjectTemplateRule> rules;
        std::vector<ProjectTemplateRule> links;
        std::vector<ProjectTemplateExport> exports;

        ProjectTemplateCleanRule clean;

        dtk::ml::MLSection* dtk;

        static std::vector<ProjectTemplate> load(const std::string& p);

        std::string to_string() const;
        ProjectCompiledTemplate compile(const std::string& name, project::build b, bool fast = false) const;
        ProjectModule get_module(const std::string& name, project::build b) const;
    };

    struct ProjectModulesFile{
        std::vector<ProjectModule> modules;

        std::string path;

        ProjectModulesFile(const std::string& path);
        void update();
    };
}