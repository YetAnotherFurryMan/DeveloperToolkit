#pragma once

#include <string>
#include <vector>

#include "defs.hpp"

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

    struct ProjectSrc{
        std::vector<std::pair<std::string, std::string>> files;
        std::vector<std::string> directories;
    };

    struct ProjectTemplate{
        std::string name;

        std::vector<std::string> directories;
        std::vector<std::pair<std::string, std::string>> files;
        std::vector<std::pair<project::extern_type, std::string>> externs;

        std::string makefile;
        std::string bash;

        ProjectTemplate() = default;

        ProjectTemplate(const std::string& path);

        void load(std::ifstream& file, const std::string& seperator);

        std::string to_string() const;
        ProjectSrc compile(const std::string& name, project::build b, bool fast = false);
    };

    std::vector<ProjectTemplate> load_project_templates(const std::string& p);

    struct ProjectTemplateFile2{
        std::string name;
        std::string cnt;
    };

    struct ProjectTemplateExport2{
        std::string name;
        project::export_type type;
    };

    struct ProjectCompiledTemplate2{
        std::vector<std::string> dirs;
        std::vector<ProjectTemplateFile2> files;
        std::vector<ProjectTemplateExport2> exports;
    };

    struct ProjectTemplateCleanRule2{
        std::vector<std::string> dirs;
        std::vector<std::string> files;
        std::vector<std::string> exes;
    };

    struct ProjectTemplateRule2{
        std::string in;
        std::string out;
        std::vector<std::string> exes;
    };

    struct ProjectTemplate2{
        std::string name;

        std::vector<std::string> dirs;
        std::vector<ProjectTemplateFile2> files;
        std::vector<ProjectTemplateRule2> rules;
        std::vector<ProjectTemplateRule2> links;
        std::vector<ProjectTemplateExport2> exports;

        ProjectTemplateCleanRule2 clean;

        static std::vector<ProjectTemplate2> load(const std::string& p);

        std::string to_string() const;
        ProjectCompiledTemplate2 compile(const std::string& name, project::build b, bool fast = false) const;
    };
}