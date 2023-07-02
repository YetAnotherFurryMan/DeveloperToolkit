#define DTK_ML_DTK_BUILDER
#define DTK_ML_STR_BUILDER
#include <fdata.hpp>

#include <fstream>
#include <sstream>

#include <cstdio>

#include "macros.hpp"

#include <common.hpp>
#include <common.h>
#include <log.hpp>

namespace fdata{
    ProjectFile::ProjectFile(const std::string& path){
        this->path = path;

        bool name_set = false;
        bool type_set = false;
        bool build_set = false;
        bool enable_set = false;

        //Open the project file
        std::ifstream project(path);
        if(!project.good())
            dtk::log::error("Failed to open project file.", 111); //EACCES

        //Read the file line-by-line
        std::string line;
        while(getline(project, line)){
            if(line.empty() || line[0] != '#') //Everythink witch does not start by the '#' is a comment
                continue;
            
            if(line.starts_with("#Name: ")){ //Load the name, return an error if redefinition
                if(name_set)
                    dtk::log::error("Project file internal error: Name redefinition.", 83); //ELOAD

                name = line.substr(7);
                name_set = true;
            } else if(line.starts_with("#Type: ")){ //Load the type, return an error if redefinition
                if(type_set)
                    dtk::log::error("Project file internal error: Type redefinition.", 83); //ELOAD

                type = (project::type) std::stoi(line.substr(7));
                type_set = true;
            } else if(line.starts_with("#Build: ")){ //Load the build system, return an error if redefinition
                if(build_set)
                    dtk::log::error("Project file internal error: Type redefinition.", 83); //ELOAD

                build = (project::build) std::stoi(line.substr(8));
                build_set = true;
            } else if(line.starts_with("#Enable: ")){ //Load the enabled functionality, return an error if redefinition
                if(enable_set)
                    dtk::log::error("Project file internal error: Enable value redefinition.", 83); //ELOAD

                enabled = std::stoi(line.substr(9));
                enable_set = true;
            } else{
                dtk::log::error("Project file internal error: Unknown \"" + line + "\"", 83); //ELOAD
            }
        }

        project.close();

        //Validate the data

        if(!name_set) //Name is required
            dtk::log::error("Project file internal error: No name definition.", 83); //ELOAD
        
        //The type and the build system have default values
        if(!type_set){
            dtk::log::warning("Project file internal error: No type definition. Default: PROJECT");
            type = project::PROJECT_TYPE_PROJECT;
        }

        if(!build_set){
            dtk::log::warning("Project file internal error: No build system definition. Default: MAKE");
            build = project::PROJECT_BUILD_MAKE;
        }
    }

    void ProjectFile::update(){
        //Update the project file
        std::ofstream project(path);
        if(!project.good())
            dtk::log::error("Failed to open project file.", 111); //EACCES
        
        project << "#Name: " << name << std::endl;
        project << "#Type: " << type << std::endl;
        project << "#Build: " << build << std::endl;
        project << "#Enable: " << enabled << std::endl;

        project.close();
    }

    std::string ProjectModule::to_string() const{
        char* str = (char*)malloc(1);
        *str = 0;

        str = dtk::ml::ml_put_section(dtk, str, "");

        return std::string(str);
    }

    std::vector<ProjectTemplate> ProjectTemplate::load(const std::string& path){
        std::vector<ProjectTemplate> t;

        dtk::ml::MLRoot* root = 0;

        //Load file
        FILE* file = fopen(path.c_str(), "r");
        if(!file){
            dtk::log::warning("Failed to open template file (\"" + path + "\").");
            return t;
        }
        
        root = dtk::ml::ml_load_file(file);

        fclose(file);

        //Validate root
        if(root->attribute_no > 0)
            dtk::log::info("Ignored root attributes in template file (\"" + path + "\").");
        
        //Load templates
        for(size_t i = 0; i < root->section_no; i++){
            if(!STRCMP_EQ(root->sections[i]->name, "template")){
                dtk::log::info("Ignored root section (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->name)) + "\") in template file (\"" + path + "\").");
                continue;
            }

            ProjectTemplate tmp;
            dtk::ml::MLSection* tmp_dtk = dtk::ml::ml_new_section();
            tmp_dtk->name = "template";

            //Validate
            if(root->sections[i]->value_no > 0)
                dtk::log::info("Ignored template values in file (\"" + path + "\").");

            //Load modifiers
            for(size_t j = 0; j < root->sections[i]->modifier_no; j++){
                char* val = dtk::common::common_str_copy(root->sections[i]->modifiers[j]->value);
                if(STRCMP_EQ(root->sections[i]->modifiers[j]->name, "name")){
                    tmp.name = val;
                    dtk::ml::ml_add_modifier(tmp_dtk, dtk::ml::ml_new_attribute("name", val));
                } else{
                    dtk::log::info("Ignored template modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->modifiers[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            //Load attributes
            for(size_t j = 0; j < root->sections[i]->attribute_no; j++){
                char* val = dtk::common::common_str_copy(root->sections[i]->attributes[j]->value);
                if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "dir")){
                    tmp.dirs.push_back(val);
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("dir", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ebin")){
                    tmp.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_BIN
                    });
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("ebin", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "elib")){
                    tmp.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_LIB
                    });
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("elib", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "einc")){
                    tmp.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_INCLUDE
                    });
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("einc", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "eres")){
                    tmp.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_RES
                    });
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("eres", val));
                } else{
                    dtk::log::info("Ignored template attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->attributes[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            //Load sections
            for(size_t j = 0; j < root->sections[i]->section_no; j++){
                if(STRCMP_EQ(root->sections[i]->sections[j]->name, "file")){
                    ProjectTemplateFile f;
                    dtk::ml::MLSection* s = dtk::ml::ml_new_section();
                    s->name = "file";

                    //Validate
                    if(root->sections[i]->sections[j]->section_no > 0)
                        dtk::log::info("Ignored file sections in file (\"" + path + "\").");
                    
                    if(root->sections[i]->sections[j]->attribute_no > 0)
                        dtk::log::info("Ignored file attributes in file (\"" + path + "\").");

                    //Load modifiers
                    for(size_t k = 0; k < root->sections[i]->sections[j]->modifier_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->value);

                        if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "name")){
                            f.name = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("name", val));
                        } else{
                            dtk::log::info("Ignored file modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    //Load values
                    std::stringstream cnt;
                    for(size_t k = 0; k < root->sections[i]->sections[j]->value_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->values[k]);
                        cnt << val;
                        dtk::ml::ml_add_value(s, val);
                    }
                    f.cnt = cnt.str();

                    tmp.files.push_back(f);
                    dtk::ml::ml_add_section(tmp_dtk, s);
                } else if(STRCMP_EQ(root->sections[i]->sections[j]->name, "rule")){
                    ProjectTemplateRule r;
                    dtk::ml::MLSection* s = dtk::ml::ml_new_section();
                    s->name = "rule";

                    //Validate
                    if(root->sections[i]->sections[j]->section_no > 0)
                        dtk::log::info("Ignored rule sections in file (\"" + path + "\").");

                    if(root->sections[i]->sections[j]->value_no > 0)
                        dtk::log::info("Ignored rule values in file (\"" + path + "\").");

                    //Load modifiers
                    for(size_t k = 0; k < root->sections[i]->sections[j]->modifier_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->value);
                        if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "name")){
                            //r.name = root->sections[i]->sections[j]->modifiers[k]->value;
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "in")){
                            r.in = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("in", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "out")){
                            r.out = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("out", val));
                        } else{
                            dtk::log::info("Ignored rule modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    //Load attributes
                    for(size_t k = 0; k < root->sections[i]->sections[j]->attribute_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->value);

                        if(STRCMP_EQ(root->sections[i]->sections[j]->attributes[k]->name, "exe")){
                            r.exes.push_back(val);
                            dtk::ml::ml_add_attribute(s, dtk::ml::ml_new_attribute("exe", val));
                        } else{
                            dtk::log::info("Ignored rule attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    tmp.rules.push_back(r);
                    dtk::ml::ml_add_section(tmp_dtk, s);
                } else if(STRCMP_EQ(root->sections[i]->sections[j]->name, "link")){
                    ProjectTemplateRule r;
                    dtk::ml::MLSection* s = dtk::ml::ml_new_section();
                    s->name = "link";

                    //Validate
                    if(root->sections[i]->sections[j]->section_no > 0)
                        dtk::log::info("Ignored link sections in file (\"" + path + "\").");

                    if(root->sections[i]->sections[j]->value_no > 0)
                        dtk::log::info("Ignored link values in file (\"" + path + "\").");

                    //Load modifiers
                    for(size_t k = 0; k < root->sections[i]->sections[j]->modifier_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->value);

                        if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "name")){
                            //r.name = root->sections[i]->sections[j]->modifiers[k]->value; //TODO
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "in")){
                            r.in = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("in", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "out")){
                            r.out = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("out", val));
                        } else{
                            dtk::log::info("Ignored link modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    //Load attributes
                    for(size_t k = 0; k < root->sections[i]->sections[j]->attribute_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->value);

                        if(STRCMP_EQ(root->sections[i]->sections[j]->attributes[k]->name, "exe")){
                            r.exes.push_back(val);
                            dtk::ml::ml_add_attribute(s, dtk::ml::ml_new_attribute("exe", val));
                        } else{
                            dtk::log::info("Ignored link attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    tmp.links.push_back(r);
                    dtk::ml::ml_add_section(tmp_dtk, s);
                } else if(STRCMP_EQ(root->sections[i]->sections[j]->name, "clean")){
                    ProjectTemplateCleanRule r;
                    dtk::ml::MLSection* s = dtk::ml::ml_new_section();
                    s->name = "clean";

                    //Validate
                    if(root->sections[i]->sections[j]->modifier_no > 0)
                        dtk::log::info("Ignored clean modifiers in file (\"" + path + "\").");

                    if(root->sections[i]->sections[j]->section_no > 0)
                        dtk::log::info("Ignored clean sections in file (\"" + path + "\").");

                    if(root->sections[i]->sections[j]->value_no > 0)
                        dtk::log::info("Ignored clean values in file (\"" + path + "\").");

                    //Load attributes
                    for(size_t k = 0; k < root->sections[i]->sections[j]->attribute_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->value);
                        if(STRCMP_EQ(root->sections[i]->sections[j]->attributes[k]->name, "dir")){
                            r.dirs.push_back(val);
                            dtk::ml::ml_add_attribute(s, dtk::ml::ml_new_attribute("dir", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->attributes[k]->name, "file")){
                            r.files.push_back(val);
                            dtk::ml::ml_add_attribute(s, dtk::ml::ml_new_attribute("file", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->attributes[k]->name, "exe")){
                            r.exes.push_back(val);
                            dtk::ml::ml_add_attribute(s, dtk::ml::ml_new_attribute("exe", val));
                        } else{
                            dtk::log::info("Ignored rule attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->attributes[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    tmp.clean = r;
                    dtk::ml::ml_add_section(tmp_dtk, s);
                } else{
                    dtk::log::info("Ignored template section (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            tmp.dtk = tmp_dtk;
            t.push_back(tmp);
        }

        dtk::ml::ml_free_root(root);

        return t;
    }

    std::string ProjectTemplate::to_string() const{
        char* str = (char*)malloc(1);
        *str = 0;

        str = dtk::ml::ml_put_section(dtk, str, "");

        return std::string(str);
    }

    void inpostprocess(std::string& s, const std::string& in,
                                     const std::string& out,
                                     const std::string& star
                    ){
        dtk::common::str::replase_all(s, "<$in>", in);
        dtk::common::str::replase_all(s, "<$out>", out);

        dtk::common::str::replase_all(s, "<*>", star);
    }

    void postprocess(std::string& s, const std::string& name,
                                     const std::string& includes, 
                                     const std::string& libs,
                                     const std::string& build
                    ){
        dtk::common::str::replase_all(s, "<$name>", name);

        dtk::common::str::replase_all(s, "<$includes>", includes);
        dtk::common::str::replase_all(s, "<$libs>", libs);
        dtk::common::str::replase_all(s, "<$build>", build);

        dtk::common::str::replase_all(s, "<$lt>", "<");
        dtk::common::str::replase_all(s, "<$gt>", ">");
    }

    ProjectCompiledTemplate ProjectTemplate::compile(const std::string& name, project::build b, bool fast) const{
        ProjectCompiledTemplate c;

        //Sending data
        std::string prefix = name + "/";

        //Add module folder
        if(!fast)
            c.dirs.push_back(name);

        for(auto& d: dirs)
            c.dirs.push_back(prefix + d);
        
        for(auto& f: files)
            c.files.emplace_back(prefix + f.name, f.cnt);
        
        //Prepare build file
        std::string build_name;
        std::stringstream build;
        if(b == project::PROJECT_BUILD_MAKE){
            build_name = "makefile";

            //Varibles
            build << "name := " << name << std::endl;
            build << std::endl;

            //All
            build << ".PHONY: all" << std::endl;
            build << "all: clean dir ";
            for(auto& e: exports)
                build << e.name << " ";
            build << std::endl << std::endl;

            //Prepare clean
            build << "clean:" << std::endl;
            build << "\t@echo Cleaning $(name)" << std::endl;
            for(auto& d: clean.dirs)
                build << "\t@rm -fr " << d << "/*" << std::endl;
            for(auto& f: clean.files)
                build << "\t@rm -f " << f << std::endl;
            for(auto e: clean.exes){
                inpostprocess(e, "", "$@", "%");
                build << "\t@" << e << std::endl;
            }
            build << std::endl;

            //Prepare directories
            build << "dir: ";
            for(auto& d: dirs)
                build << d << " ";
            build << std::endl << std::endl;
            for(auto& d: dirs)
                build << d << " ";
            build << ":" << std::endl;
            build << "\t@mkdir $@" << std::endl;
            build << std::endl;

            //Prepare rules and in and out varibles
            std::string var_in = "in := ";
            std::string var_out = "out := ";
            std::stringstream tmp;
            for(auto& r: rules){
                std::string rule = r.out + ": " + r.in;
                inpostprocess(rule, "", "", "%");

                tmp << rule << std::endl;
                tmp << "\t@echo Compiling $^" << std::endl;
                for(auto e: r.exes){
                    inpostprocess(e, "$^", "$@", "%");
                    tmp << "\t@" << e << std::endl;
                }
                tmp << std::endl;

                //Add to in and out
                auto in = r.in;
                inpostprocess(in, "", "", "*");

                var_in += "$(wildcard " + in + ") ";
                var_out += "$(patsubst " + r.in + "," + r.out + ",$(wildcard " + in + ")) ";
            }

            inpostprocess(var_out, "", "", "%");

            build << var_in << std::endl;
            build << var_out << std::endl;
            build << std::endl;

            build << tmp.str();

            //Prepare links
            for(auto& l: links){
                auto in = l.in;
                inpostprocess(in, "", "", "%");

                build << l.out << ": $(filter " << in << ", $(out))" << std::endl;
                build << "\t@echo Linking $@" << std::endl;
                for(auto e: l.exes){
                    inpostprocess(e, "$^", "$@", "%");
                    build << "\t@" << e << std::endl;
                }
                build << std::endl;
            }
        } else if(b == project::PROJECT_BUILD_BASH){
            if(fast)
                build_name = ".scripts/";
            build_name += "make.sh";

            //Varibles
            build << "name=" << name << std::endl;
            build << std::endl;

            //Prepare clean
            build << "#Clean:" << std::endl;
            build << "echo Cleaning $name" << std::endl;
            for(auto& d: clean.dirs)
                build << "rm -fr " << d << "/*" << std::endl;
            for(auto& f: clean.files)
                build << "rm -f " << f << std::endl;
            for(auto e: clean.exes){
                inpostprocess(e, "", "clean", "*");
                build << e << std::endl;
            }
            build << std::endl;

            //Prepare directories
            build << "#Directories:" << std::endl;
            for(auto& d: dirs)
                build << "mkdir " << d << " 2>/dev/null" << std::endl;
            build << std::endl;

            //Prepare rules
            build << "#Rules:" << std::endl;
            build << "shopt -s nullglob" << std::endl;
            build << std::endl;
            for(auto& r: rules){
                auto in = r.in;
                inpostprocess(in, "", "", "*");

                build << "set -- " << in << std::endl;
                build << "if [ \"$#\" -gt 0 ]; then" << std::endl;
                build << "\tfor file in " << in << "; do" << std::endl;
                build << "\t\techo Compiling $file" << std::endl;

                //Calculating out varible
                auto out = r.out;
                auto star = r.in.find("<*>");
                dtk::common::str::replase_all(out, "<*>", std::string("${file#\"") + r.in.substr(0, star) + "\"};out=${out%\"" + r.in.substr(star + 3) + "\"}");
                build << "\t\tout=" << out << std::endl;

                for(auto e: r.exes){
                    inpostprocess(e, "$file", "$out", "*");
                    build << "\t\t" << e << std::endl;
                }

                build << "\tdone" << std::endl;
                build << "fi" << std::endl;
                build << std::endl;
            }

            build << "shopt -u nullglob" << std::endl;
            build << std::endl;

            //Prepare links
            build << "#Links:" << std::endl;
            build << std::endl;
            for(auto& l: links){
                build << "echo Linking " << l.out << std::endl;
                for(auto e: l.exes){
                    inpostprocess(e, l.in, l.out, "*");
                    build << e << std::endl;
                }
                build << std::endl;
            }
        }

        c.files.emplace_back(prefix + build_name, build.str());

        //Posprocess
        std::string var_name = name;
        if(b == project::PROJECT_BUILD_MAKE)
            var_name = "$(name)";
        else if(b == project::PROJECT_BUILD_BASH)
            var_name = "$name";
        
        std::string var_includes = (!fast)?"-I ../includes":"";
        std::string var_libs = (!fast)?"-L ../libs":"";

        for(auto& d: c.dirs)
            postprocess(d, var_name, var_includes, var_libs, std::to_string(b));

        for(auto& f: c.files){
            postprocess(f.name, var_name, var_includes, var_libs, std::to_string(b));
            postprocess(f.cnt, var_name, var_includes, var_libs, std::to_string(b));
        }

        return c;
    }

    ProjectModule ProjectTemplate::get_module(const std::string& name, project::build b) const{
        ProjectModule m;
        m.name = name;
        m.tmpl_name = this->name;

        dtk::ml::MLSection* m_dtk = dtk::ml::ml_new_section();
        m_dtk->name = "module";

        dtk::ml::ml_add_modifier(m_dtk, dtk::ml::ml_new_attribute("name", name.c_str()));
        dtk::ml::ml_add_modifier(m_dtk, dtk::ml::ml_new_attribute("template", this->name.c_str()));

        for(auto e: exports){
            postprocess(e.name, name, "", "", std::to_string(b));
            m.exports.push_back(e);

            char* att_name = "ebin";
            switch(e.type){
                case project::PROJECT_EXPORT_LIB:
                    att_name = "elib";
                    break;
                case project::PROJECT_EXPORT_INCLUDE:
                    att_name = "einc";
                    break;
                case project::PROJECT_EXPORT_RES:
                    att_name = "eres";
                    break;
            }

            dtk::ml::ml_add_attribute(m_dtk, dtk::ml::ml_new_attribute(att_name, e.name.c_str()));
        }

        m.dtk = m_dtk;

        return m;
    }

    ProjectModulesFile::ProjectModulesFile(const std::string& path){
        this->path = path;

        dtk::ml::MLRoot* root;

        //Load the modules' file root node
        FILE* file = fopen(path.c_str(), "r");
        if(!file)
            dtk::log::warning("Failed to open template file (\"" + path + "\").");
        
        root = dtk::ml::ml_load_file(file);

        fclose(file);

        //Validate root
        if(root->attribute_no > 0)
            dtk::log::warning("Ignored root attributes in module file (\"" + path + "\").");
        
        if(root->definition_no > 0)
            dtk::log::warning("Ignored root definitions in module file (\"" + path + "\").");

        //Load templates
        for(size_t i = 0; i < root->section_no; i++){
            if(!STRCMP_EQ(root->sections[i]->name, "module")){
                dtk::log::warning("Ignored root section (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->name)) + "\") in module file (\"" + path + "\").");
                continue;
            }

            ProjectModule mod;
            dtk::ml::MLSection* mod_dtk = dtk::ml::ml_new_section();
            mod_dtk->name = "module";

            //Validate
            if(root->sections[i]->value_no > 0)
                dtk::log::warning("Ignored the module's values in file (\"" + path + "\").");
            
            if(root->sections[i]->section_no > 0)
                dtk::log::warning("Ignored the module's sections in file (\"" + path + "\").");

            //Load modifiers
            for(size_t j = 0; j < root->sections[i]->modifier_no; j++){
                char* val = dtk::common::common_str_copy(root->sections[i]->modifiers[j]->value);
                if(STRCMP_EQ(root->sections[i]->modifiers[j]->name, "name")){
                    mod.name = val;
                    dtk::ml::ml_add_modifier(mod_dtk, dtk::ml::ml_new_attribute("name", val));
                } else if(STRCMP_EQ(root->sections[i]->modifiers[j]->name, "template")){
                    mod.tmpl_name = val;
                    dtk::ml::ml_add_modifier(mod_dtk, dtk::ml::ml_new_attribute("template", val));
                } else{
                    dtk::log::warning("Ignored module's modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->modifiers[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            //Load attributes
            for(size_t j = 0; j < root->sections[i]->attribute_no; j++){
                char* val = dtk::common::common_str_copy(root->sections[i]->attributes[j]->value);
                if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ebin")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_BIN
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ebin", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "elib")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_LIB
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("elib", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "einc")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_INCLUDE
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("einc", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "eres")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_EXPORT_RES
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("eres", val));
                } else{
                    dtk::log::info("Ignored module's attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->attributes[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            mod.dtk = mod_dtk;
            this->modules.push_back(mod);
        }
    }

    void ProjectModulesFile::update(){
        //Open modules file
        std::ofstream file(path);
        if(!file.good())
            dtk::log::error("Failed to open modules file.", 111); //EACCES
        
        //Write the data
        for(auto& m: modules){
            file << "<!-- Module: " << m.name << " -->" << std::endl;
            file << m.to_string();
            file << std::endl;
        }

        //Close file
        file.close();
    }
}