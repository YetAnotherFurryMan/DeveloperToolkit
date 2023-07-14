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

    std::vector<std::string> ProjectTemplateRule::compile_exes(const std::vector<ProjectTemplateItem> items) const{
        std::vector<std::string> out;
        
        //Varibles
        std::string _bins = "";
        std::string _libs = "";
        std::string _incs = "";
        std::string _ress = "";
        std::string _bins_raw = "";
        std::string _libs_raw = "";
        std::string _incs_raw = "";
        std::string _ress_raw = "";
        std::string _bin_dirs = "";
        std::string _lib_dirs = "";
        std::string _inc_dirs = "";
        std::string _res_dirs = "";
        std::string _bin_dirs_raw = "";
        std::string _lib_dirs_raw = "";
        std::string _inc_dirs_raw = "";
        std::string _res_dirs_raw = "";
        
        //Load
        for(auto& item: items){
            switch(item.type){
                case project::PROJECT_ITEM_BIN:
                    _bins += bin_prefix + item.name + " ";
                    _bins_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_LIB:
                    _libs += lib_prefix + item.name + " ";
                    _libs_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_INC:
                    _incs += inc_prefix + item.name + " ";
                    _incs_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_RES:
                    _ress += res_prefix + item.name + " ";
                    _ress_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_BIN_DIR:
                    _bin_dirs += bin_dir_prefix + item.name + " ";
                    _bin_dirs_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_LIB_DIR:
                    _lib_dirs += lib_dir_prefix + item.name + " ";
                    _lib_dirs_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_INC_DIR:
                    _inc_dirs += inc_dir_prefix + item.name + " ";
                    _inc_dirs_raw += item.name + " ";
                    break;
                case project::PROJECT_ITEM_RES_DIR:
                    _res_dirs += res_dir_prefix + item.name + " ";
                    _res_dirs_raw += item.name + " ";
                    break;
            }
        }

        //Replase
        for(auto e: exes){
            dtk::common::str::replase_all(e, "<$bins>", _bins);
            dtk::common::str::replase_all(e, "<$libs>", _libs);
            dtk::common::str::replase_all(e, "<$incs>", _incs);
            dtk::common::str::replase_all(e, "<$ress>", _ress);

            dtk::common::str::replase_all(e, "<$bins_raw>", _bins_raw);
            dtk::common::str::replase_all(e, "<$libs_raw>", _libs_raw);
            dtk::common::str::replase_all(e, "<$incs_raw>", _incs_raw);
            dtk::common::str::replase_all(e, "<$ress_raw>", _ress_raw);

            dtk::common::str::replase_all(e, "<$bin_dirs>", _bin_dirs);
            dtk::common::str::replase_all(e, "<$lib_dirs>", _lib_dirs);
            dtk::common::str::replase_all(e, "<$inc_dirs>", _inc_dirs);
            dtk::common::str::replase_all(e, "<$res_dirs>", _res_dirs);

            dtk::common::str::replase_all(e, "<$bin_dirs_raw>", _bin_dirs_raw);
            dtk::common::str::replase_all(e, "<$lib_dirs_raw>", _lib_dirs_raw);
            dtk::common::str::replase_all(e, "<$inc_dirs_raw>", _inc_dirs_raw);
            dtk::common::str::replase_all(e, "<$res_dirs_raw>", _res_dirs_raw);

            dtk::common::str::replase_all(e, "<$bins_prefix>", bin_prefix);
            dtk::common::str::replase_all(e, "<$libs_prefix>", lib_prefix);
            dtk::common::str::replase_all(e, "<$incs_prefix>", inc_prefix);
            dtk::common::str::replase_all(e, "<$ress_prefix>", res_prefix);

            dtk::common::str::replase_all(e, "<$bin_dirs_prefix>", bin_dir_prefix);
            dtk::common::str::replase_all(e, "<$lib_dirs_prefix>", lib_dir_prefix);
            dtk::common::str::replase_all(e, "<$inc_dirs_prefix>", inc_dir_prefix);
            dtk::common::str::replase_all(e, "<$res_dirs_prefix>", res_dir_prefix);

            out.push_back(e);
        }

        return out;
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

                #define IF_ATTRIBUTE(ATT, VAR, TYPE) if(STRCMP_EQ(root->sections[i]->attributes[j]->name, ATT)){ tmp.VAR.push_back({ val, project::PROJECT_ITEM_##TYPE }); dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute(ATT, val)); }

                if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "dir")){
                    tmp.dirs.push_back(val);
                    dtk::ml::ml_add_attribute(tmp_dtk, dtk::ml::ml_new_attribute("dir", val));
                } else IF_ATTRIBUTE("ebin", exports, BIN)
                else IF_ATTRIBUTE("elib", exports, LIB)
                else IF_ATTRIBUTE("einc", exports, INC)
                else IF_ATTRIBUTE("eres", exports, RES)
                else IF_ATTRIBUTE("ibin", imports, BIN)
                else IF_ATTRIBUTE("ilib", imports, LIB)
                else IF_ATTRIBUTE("iinc", imports, INC)
                else IF_ATTRIBUTE("ires", imports, RES)
                else IF_ATTRIBUTE("ibin_dir", imports, BIN_DIR)
                else IF_ATTRIBUTE("ilib_dir", imports, LIB_DIR)
                else IF_ATTRIBUTE("iinc_dir", imports, INC_DIR)
                else IF_ATTRIBUTE("ires_dir", imports, RES_DIR)
                else{
                    dtk::log::info("Ignored template attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->attributes[j]->name)) + "\") in file (\"" + path + "\").");
                }

                #undef IF_ATTRIBUTE
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
                            r.name = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("name", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "in")){
                            r.in = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("in", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "out")){
                            r.out = val;
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("out", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "type")){
                            if(STRCMP_EQ(val, "121"))
                                r.type = project::PROJECT_RULE_121;
                            else if(STRCMP_EQ(val, "N21"))
                                r.type = project::PROJECT_RULE_N21;
                            else if(STRCMP_EQ(val, "12N"))
                                r.type = project::PROJECT_RULE_12N;
                            else if(STRCMP_EQ(val, "N2N"))
                                r.type = project::PROJECT_RULE_N2N;
                            else
                                dtk::log::error("Bad rule type '" + std::string(val) + "'", 83); //ELOAD
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("type", val));
                        } else if(STRCMP_EQ(root->sections[i]->sections[j]->modifiers[k]->name, "priority")){
                            try{
                                r.priority = std::stoi(val);
                            } catch(std::exception &e){
                                std::string exc_str = e.what();
                                dtk::log::error("Priority conversion to int failed.\n\twhat(): " + exc_str);
                            }
                            dtk::ml::ml_add_modifier(s, dtk::ml::ml_new_attribute("priority", std::to_string(r.priority).c_str()));
                        } else{
                            dtk::log::info("Ignored rule modifier (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->sections[j]->modifiers[k]->name)) + "\") in file (\"" + path + "\").");
                        }
                    }

                    //Load definitions
                    for(size_t k = 0; k < root->sections[i]->sections[j]->definition_no; k++){
                        char* val = dtk::common::common_str_copy(root->sections[i]->sections[j]->definitions[k]->value);

                        #define IF_PREFIX(PREFIX, VAR) if(STRCMP_EQ(root->sections[i]->sections[j]->definitions[k]->name, PREFIX)){ VAR = val; dtk::ml::ml_add_definition(s, dtk::ml::ml_new_attribute(PREFIX, val)); }
                        
                        IF_PREFIX("bin_prefix", r.bin_prefix)
                        else IF_PREFIX("lib_prefix", r.lib_prefix)
                        else IF_PREFIX("inc_prefix", r.inc_prefix)
                        else IF_PREFIX("res_prefix", r.res_prefix)
                        else IF_PREFIX("bin_dir_prefix", r.bin_dir_prefix)
                        else IF_PREFIX("lib_dir_prefix", r.lib_dir_prefix)
                        else IF_PREFIX("inc_dir_prefix", r.inc_dir_prefix)
                        else IF_PREFIX("res_dir_prefix", r.res_dir_prefix)
                        else IF_PREFIX("in_prefix", r.in_prefix)
                        else IF_PREFIX("out_prefix", r.out_prefix)

                        #undef IF_PREFIX
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

    void postprocess(
        std::string& s,
        const std::string& name,
        const std::string& build
    ){
        dtk::common::str::replase_all(s, "<$name>", name);
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
        
        //Add build file
        c.files.push_back(compile_rules(name, b, fast));

        //Posprocess
        std::string var_name = name;
        if(b == project::PROJECT_BUILD_MAKE)
            var_name = "$(name)";
        else if(b == project::PROJECT_BUILD_BASH)
            var_name = "$name";

        for(auto& d: c.dirs)
            postprocess(d, var_name, std::to_string(b));

        for(auto& f: c.files){
            postprocess(f.name, var_name, std::to_string(b));
            postprocess(f.cnt, var_name, std::to_string(b));
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
            postprocess(e.name, name, std::to_string(b));
            m.exports.push_back(e);

            char* att_name = "ebin";
            switch(e.type){
                case project::PROJECT_ITEM_LIB:
                    att_name = "elib";
                    break;
                case project::PROJECT_ITEM_INC:
                    att_name = "einc";
                    break;
                case project::PROJECT_ITEM_RES:
                    att_name = "eres";
                    break;
            }

            dtk::ml::ml_add_attribute(m_dtk, dtk::ml::ml_new_attribute(att_name, e.name.c_str()));
        }

        for(auto e: imports){
            postprocess(e.name, name, std::to_string(b));
            m.imports.push_back(e);

            char* att_name = "ibin";
            switch(e.type){
                case project::PROJECT_ITEM_LIB:
                    att_name = "ilib";
                    break;
                case project::PROJECT_ITEM_INC:
                    att_name = "iinc";
                    break;
                case project::PROJECT_ITEM_RES:
                    att_name = "ires";
                    break;
                case project::PROJECT_ITEM_BIN_DIR:
                    att_name = "ibin_dir";
                    break;
                case project::PROJECT_ITEM_LIB_DIR:
                    att_name = "ilib_dir";
                    break;
                case project::PROJECT_ITEM_INC_DIR:
                    att_name = "iinc_dir";
                    break;
                case project::PROJECT_ITEM_RES_DIR:
                    att_name = "ires_dir";
                    break;
            }

            dtk::ml::ml_add_attribute(m_dtk, dtk::ml::ml_new_attribute(att_name, e.name.c_str()));
        }

        m.dtk = m_dtk;

        return m;
    }

    ProjectTemplateFile ProjectTemplate::compile_rules(const std::string& name, project::build b, bool fast) const{
        auto r = compile_rules(name, b, fast, *this, this->imports);
        r.name = name + "/" + r.name;
        return r;
    }

    ProjectTemplateFile ProjectTemplate::compile_rules(const std::string& name, project::build b, bool fast, const ProjectTemplate& temp, const std::vector<ProjectTemplateItem>& imp){
        //Complete imports if !fast
        auto imports = imp;
        if(!fast){
            imports.push_back({"../bin", project::PROJECT_ITEM_BIN_DIR});
            imports.push_back({"../lib", project::PROJECT_ITEM_LIB_DIR});
            imports.push_back({"../include", project::PROJECT_ITEM_INC_DIR});
            imports.push_back({"../res", project::PROJECT_ITEM_RES_DIR});
        }

        ProjectTemplateFile file;

        //Choose file name
        switch(b){
            case project::PROJECT_BUILD_MAKE:
                file.name = "makefile";
                break;
            case project::PROJECT_BUILD_BASH:
                if(fast)
                    file.name = ".scripts/make.sh";
                else
                    file.name = "make.sh";
                break;
        }

        //Compile rules to file
        auto rules = temp.rules;
        
        std::stringstream ss;

        if(b == project::PROJECT_BUILD_MAKE){
            //Rules, in and out
            std::string in = "in :=";
            std::string out = "out :=";
            std::stringstream compiled;
            
            for(size_t i = 0; i < rules.size(); i++){
                int rule = 0;
                for(size_t j = 0; j < rules.size(); j++){
                    if(rules[rule].priority < rules[j].priority)
                        rule = j;
                    else if(rules[rule].type < rules[j].type && rules[rule].priority == rules[j].priority)
                        rule = j;
                }

                std::string wildcard = "$(wildcard " + rules[rule].in + ")";
                dtk::common::str::replase_all(wildcard, "<*>", "*");
                in += " " + wildcard;
                if(rules[rule].type == project::PROJECT_RULE_12N || rules[rule].type == project::PROJECT_RULE_N2N)
                    out += " $(patsubst " + rules[rule].in + "," + rules[rule].out + "," + wildcard + ")";
                else
                    out += " " + rules[rule].out;

                std::string label = rules[rule].out;
                if(rules[rule].type == project::PROJECT_RULE_N21)
                    label += ": $(filter " + rules[rule].in + ", $(out) $(in))";
                else
                    label += ": " + rules[rule].in;
                dtk::common::str::replase_all(label, "<*>", "%");

                if(!rules[rule].name.empty())
                    compiled << rules[rule].name << ": ";
                compiled << label << std::endl;

                std::string _in = "$^";
                if(rules[rule].in_prefix != ""){
                    if(rules[rule]. type == project::PROJECT_RULE_N21 || rules[rule].type == project::PROJECT_RULE_N2N)
                        _in = "$(foreach x," + rules[rule].in_prefix + "$(x), $^)";
                    else
                        _in = rules[rule].in_prefix + "$^";
                }

                std::string _out = rules[rule].out_prefix + "$@";
                if(rules[rule].out_prefix != ""){
                    _out = rules[rule].out_prefix + "$@";
                }

                auto exes = rules[rule].compile_exes(imports);
                
                for(auto& exe: exes){
                    dtk::common::str::replase_all(exe, "<$in>", _in);
                    dtk::common::str::replase_all(exe, "<$out>", _out);
                    dtk::common::str::replase_all(exe, "<*>", "*");

                    compiled << "\t@" << exe << std::endl;
                }

                compiled << std::endl;

                rules[rule].priority = -1;
            }

            dtk::common::str::replase_all(out, "<*>", "%");

            //Head
            ss << "name := " << name << std::endl;
            ss << in << std::endl;
            ss << out << std::endl;
            ss << std::endl;
            ss << ".PHONY: all" << std::endl;
            ss << "all: clean dir $(out)" << std::endl;
            ss << std::endl;

            //Clean
            ss << "clean:" << std::endl;
            ss << "\t@echo Cleaning $(name)" << std::endl;

            for(auto& d: temp.clean.dirs)
                ss << "\t@rm -fr " << d << std::endl;

            for(auto& f: temp.clean.files)
                ss << "\t@rm -f " << f << std::endl;

            for(auto e: temp.clean.exes){
                dtk::common::str::replase_all(e, "<*>", "*");
                ss << "\t@" << e << std::endl;
            }

            ss << std::endl;

            //Directories
            ss << "dir: ";

            for(auto& d: temp.dirs)
                ss << d << " ";

            ss << std::endl << std::endl;

            for(auto& d: temp.dirs)
                ss << d << " ";

            ss << ":" << std::endl;
            ss << "\t@mkdir $@" << std::endl;
            ss << std::endl;

            //Add compiled rules
            ss << compiled.str();
        } else if(b == project::PROJECT_BUILD_BASH){
            //Head
            ss << "name=" << name << std::endl;
            ss << std::endl;

            //Clean
            ss << "#Clean:" << std::endl;
            ss << "echo Cleaning $name" << std::endl;

            for(auto& d: temp.clean.dirs)
                ss << "rm -fr " << d << std::endl;

            for(auto& f: temp.clean.files)
                ss << "rm -f " << f << std::endl;

            for(auto e: temp.clean.exes){
                dtk::common::str::replase_all(e, "<*>", "*");
                ss << e << std::endl;
            }

            ss << std::endl;

            //Directories
            ss << "#Directories:" << std::endl;

            for(auto& d: temp.dirs)
                ss << "mkdir " << d << " 2>/dev/null" << std::endl;

            ss << std::endl;

            //Rules
            ss << "#Rules:" << std::endl;
            ss << "shopt -s nullglob" << std::endl;
            ss << std::endl;

            for(size_t i = 0; i < rules.size(); i++){
                int rule = 0;
                for(size_t j = 0; j < rules.size(); j++){
                    if(rules[rule].priority < rules[j].priority)
                        rule = j;
                    else if(rules[rule].type < rules[j].type && rules[rule].priority == rules[j].priority)
                        rule = j;
                }

                auto exes = rules[rule].compile_exes(imports);

                switch(rules[rule].type){
                    case project::PROJECT_RULE_121:
                    case project::PROJECT_RULE_N21:
                        for(auto& e: exes){
                            dtk::common::str::replase_all(e, "<$in>", rules[rule].in_prefix + rules[rule].in);
                            dtk::common::str::replase_all(e, "<$out>", rules[rule].out_prefix + rules[rule].out);

                            ss << e << std::endl;
                        }
                        break;
                    case project::PROJECT_RULE_12N:
                    case project::PROJECT_RULE_N2N:
                        ss << "set -- " << rules[rule].in << std::endl;
                        ss << "if [ \"$#\" -gt 0 ]" << std::endl;
                        ss << "then" << std::endl;
                        ss << "\tfor file in " << rules[rule].in << std::endl;
                        ss << "\tdo" << std::endl;

                        //Out varible
                        auto out = rules[rule].out;
                        auto star = rules[rule].in.find("<*>");
                        dtk::common::str::replase_all(out, "<*>", std::string("${file#\"") + rules[rule].in.substr(0, star) + "\"};out=${out%\"" + rules[rule].in.substr(star + 3) + "\"}");
                        
                        ss << "\t\tout=" << out << std::endl;

                        for(auto& e: exes){
                            dtk::common::str::replase_all(e, "<$in>", rules[rule].in_prefix + "$file");
                            dtk::common::str::replase_all(e, "<$out>", rules[rule].out_prefix + "$out");

                            ss << "\t\t" << e << std::endl;
                        }

                        ss << "\tdone" << std::endl;
                        ss << "fi" << std::endl;
                        break;
                }

                ss << std::endl;
                rules[rule].priority = -1;
            }

            ss << "shopt -u nullglob" << std::endl;
            ss << std::endl;
        }

        file.cnt = ss.str();
        dtk::common::str::replase_all(file.cnt, "<*>", "*");

        return file;
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
                        project::PROJECT_ITEM_BIN
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ebin", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "elib")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_ITEM_LIB
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("elib", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "einc")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_ITEM_INC
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("einc", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "eres")){
                    mod.exports.push_back({
                        val, 
                        project::PROJECT_ITEM_RES
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("eres", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ibin")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_BIN
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ibin", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ilib")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_LIB
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ilib", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "iinc")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_INC
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("iinc", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ires")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_RES
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ires", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ibin_dir")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_BIN_DIR
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ibin_dir", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ilib_dir")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_LIB_DIR
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ilib_dir", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "iinc_dir")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_INC_DIR
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("iinc_dir", val));
                } else if(STRCMP_EQ(root->sections[i]->attributes[j]->name, "ires_dir")){
                    mod.imports.push_back({
                        val, 
                        project::PROJECT_ITEM_RES_DIR
                    });
                    dtk::ml::ml_add_attribute(mod_dtk, dtk::ml::ml_new_attribute("ires_dir", val));
                } else{
                    dtk::log::info("Ignored module's attribute (\"" + std::string(dtk::common::common_str_copy(root->sections[i]->attributes[j]->name)) + "\") in file (\"" + path + "\").");
                }
            }

            mod.dtk = mod_dtk;
            this->modules.push_back(mod);
        }
    }

    std::string ProjectModule::to_string() const{
        char* str = (char*)malloc(1);
        *str = 0;

        str = dtk::ml::ml_put_section(dtk, str, "");

        return std::string(str);
    }

    void ProjectModule::add_import(const std::string& val, project::item_type t, project::build b, const ProjectTemplate& tmpl){
        if(tmpl.name != tmpl_name)
            return;
        
        const char* att = "ibin";
        
        switch(t){
            case project::PROJECT_ITEM_LIB:
                att = "ilib";
                break;
            case project::PROJECT_ITEM_INC:
                att = "iinc";
                break;
            case project::PROJECT_ITEM_RES:
                att = "ires";
                break;
            case project::PROJECT_ITEM_BIN_DIR:
                att = "ibin_dir";
                break;
            case project::PROJECT_ITEM_LIB_DIR:
                att = "ilib_dir";
                break;
            case project::PROJECT_ITEM_INC_DIR:
                att = "iinc_dir";
                break;
            case project::PROJECT_ITEM_RES_DIR:
                att = "ires_dir";
                break;
        }

        //Add import
        imports.push_back({val, t});
        dtk::ml::ml_add_attribute(dtk, dtk::ml::ml_new_attribute(att, val.c_str()));

        //Update build file
        auto file = ProjectTemplate::compile_rules(name, b, false, tmpl, imports);
        file.name = name + "/" + file.name;

        //Posprocess
        std::string var_name = name;
        if(b == project::PROJECT_BUILD_MAKE)
            var_name = "$(name)";
        else if(b == project::PROJECT_BUILD_BASH)
            var_name = "$name";

        postprocess(file.name, var_name, std::to_string(b));
        postprocess(file.cnt, var_name, std::to_string(b));
                                    
        std::ofstream ofs(file.name);
        FILE_OK(ofs)
        ofs << file.cnt;
        ofs.close();
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