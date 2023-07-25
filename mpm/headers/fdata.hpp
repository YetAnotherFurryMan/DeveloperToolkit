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

    struct ProjectTemplateItem{
        std::string name;
        project::item_type type;
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
        std::string name;
        std::string title;
        project::rule_type type = project::PROJECT_RULE_N2N;
        int priority = 0;
        
        std::string in;
        std::string out;
        std::vector<std::string> exes;

        std::string bin_prefix = "";
        std::string lib_prefix = "";
        std::string inc_prefix = "";
        std::string res_prefix = "";

        std::string bin_dir_prefix = "";
        std::string lib_dir_prefix = "";
        std::string inc_dir_prefix = "";
        std::string res_dir_prefix = "";

        std::string in_prefix = "";
        std::string out_prefix = "";

        std::vector<std::string> compile_exes(const std::vector<ProjectTemplateItem> items) const;
    };

    struct ProjectModule;

    struct ProjectTemplate{
        std::string name;

        std::vector<std::string> dirs;
        std::vector<ProjectTemplateFile> files;
        std::vector<ProjectTemplateRule> rules;
        std::vector<ProjectTemplateItem> exports;
        std::vector<ProjectTemplateItem> imports;

        ProjectTemplateCleanRule clean;

        dtk::ml::MLSection* dtk;

        static std::vector<ProjectTemplate> load(const std::string& p);

        std::string to_string() const;
        ProjectCompiledTemplate compile(const std::string& name, project::build b, bool fast = false) const;
        ProjectModule get_module(const std::string& name, project::build b) const;

        ProjectTemplateFile compile_rules(const std::string& name, project::build b, bool fast) const;
        static ProjectTemplateFile compile_rules(const std::string& name, project::build b, bool fast, const ProjectTemplate& template_file, const std::vector<ProjectTemplateItem>& imports);
    };

    struct ProjectModule{
        std::string name;
        std::string tmpl_name;
        std::vector<ProjectTemplateItem> exports;
        std::vector<ProjectTemplateItem> imports;

        dtk::ml::MLSection* dtk;

        std::string to_string() const;
        void add_import(const std::string& val, project::item_type type, project::build build, const ProjectTemplate& tmpl);
    };

    struct ProjectModulesFile{
        std::vector<ProjectModule> modules;

        std::string path;

        ProjectModulesFile(const std::string& path);
        void update();
    };
}