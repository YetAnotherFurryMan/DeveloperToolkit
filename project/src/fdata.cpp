#include <fdata.hpp>

#include <fstream>
#include <sstream>

#include <common.hpp>

#include <liblog.hpp>

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
            dtk::log::error("Failed to open project file.", 83); //ELOAD

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
            dtk::log::error("Failed to open project file.", 83); //ELOAD
        
        project << "#Name: " << name << std::endl;
        project << "#Type: " << type << std::endl;
        project << "#Build: " << build << std::endl;
        project << "#Enable: " << enabled << std::endl;

        project.close();
    }

    ProjectTemplate::ProjectTemplate(const std::string& path){
        //Open template file
        std::ifstream file(path);
        if(!file.good())
            dtk::log::error("Failed to open template file.", 83); //ELOAD

        //Load the file to EOF or '#End' (Unused in this case)
        load(file, "End");

        file.close();
    }

    void ProjectTemplate::load(std::ifstream& file, const std::string& seperator){
        //Loads template data

        bool name_set = false;

        bool makefile_set = false;
        bool bash_set = false;

        std::string line;

        //Load data
        while(getline(file, line)){
            if(line.empty() || !line.starts_with("#")) //Everything witch doesn't start with '#' is a comment in this case
                continue;

            if(line.starts_with("#" + seperator)) //Read only to seperator
                break;
            
            if(line.starts_with("#Name: ")){ //Get name, return an error if redefinition
                if(name_set)
                    dtk::log::error("Template file internal error: Name redefinition.", 83); //ELOAD

                name = line.substr(7);
                name_set = true;
            } else if(line.starts_with("#!DIR: ")){ //Add a directory, return a warning if already exists
                std::string dir = line.substr(7);

                bool found = false;
                for(auto& d: directories){
                    if(d == dir){
                        dtk::log::error("Template file internal error: DIR already exists.");
                        found = true;
                        break;
                    }
                }

                if(!found)
                    directories.push_back(dir);
            } else if(line.starts_with("#!EBIN: ")){ //Add a extern binary, return a warning if already exists
                std::string file = line.substr(8);

                bool found = false;
                for(auto& e: externs){
                    if(e.first != project::PROJECT_EXTERN_BIN)
                        continue;
                    
                    if(e.second == file){
                        dtk::log::error("Template file internal error: EBIN already exists.");
                        found = true;
                        break;
                    }
                }

                if(!found)
                    externs.emplace_back(project::PROJECT_EXTERN_BIN, file);
            } else if(line.starts_with("#!ELIB: ")){ //Add a extern liblary, return a warning if already exists
                std::string file = line.substr(8);

                bool found = false;
                for(auto& e: externs){
                    if(e.first != project::PROJECT_EXTERN_LIB)
                        continue;
                    
                    if(e.second == file){
                        dtk::log::error("Template file internal error: ELIB already exists.");
                        found = true;
                        break;
                    }
                }

                if(!found)
                    externs.emplace_back(project::PROJECT_EXTERN_LIB, file);
            } else if(line.starts_with("#!EINC: ")){ //Add a extern include, return a warning if already exists
                std::string file = line.substr(8);

                bool found = false;
                for(auto& e: externs){
                    if(e.first != project::PROJECT_EXTERN_INCLUDE)
                        continue;
                    
                    if(e.second == file){
                        dtk::log::error("Template file internal error: EINC already exists.");
                        found = true;
                        break;
                    }
                }

                if(!found)
                    externs.emplace_back(project::PROJECT_EXTERN_INCLUDE, file);
            } else if(line.starts_with("#!FILE: ")){ //Get text to '#!END' and interpret as file, if filename == makefile or bash get it as special case
                std::string file_name = line.substr(8);

                std::stringstream ss;

                while(getline(file, line)){
                    if(line.starts_with("#!END"))
                        break;
                    
                    ss << line << std::endl;
                }

                if(file_name == "makefile"){ //makefile case
                    if(makefile_set)
                        dtk::log::error("Template file internal error: makefile redefinition.", 83); //ELOAD

                    makefile = ss.str();
                    makefile_set = true;
                } else if(file_name == "bash"){ //bash case
                    if(bash_set)
                        dtk::log::error("Template file internal error: bash redefinition.", 83); //ELOAD
                    
                    bash = ss.str();
                    bash_set = true;
                } else{ //file case
                    files.push_back(std::pair<std::string, std::string>(file_name, ss.str()));
                }
            } else{
                dtk::log::error("Template file internal error: Unknown \"" + line + "\".", 83); //ELOAD
            }
        }

        //Validate the data

        if(files.size() <= 0)
            dtk::log::error("Template file internal error: Excepted file.", 83); //ELOAD
        
        if(!makefile_set)
            dtk::log::error("Template file internal error: makefile undefined.", 83); //ELOAD

        if(!bash_set)
            dtk::log::error("Template file internal error: bash undefined.", 83); //ELOAD
    }
    
    std::string ProjectTemplate::to_string() const{
        //Restore the template to file-form

        std::stringstream ss;
        ss << "#Name: " << name << std::endl;
        ss << std::endl;

        for(auto& dir: directories)
            ss << "#!DIR: " << dir << std::endl;
        
        ss << std::endl;

        for(auto& e: externs){
            switch(e.first){
                case project::PROJECT_EXTERN_BIN:
                    ss << "#!EBIN: ";
                    break;
                case project::PROJECT_EXTERN_LIB:
                    ss << "#!ELIB: ";
                    break;
                case project::PROJECT_EXTERN_INCLUDE:
                    ss << "#!EINC: ";
                    break;
            }

            ss << e.second << std::endl;
        }

        ss << std::endl;
        
        for(auto& file: files){
            ss << "#!FILE: " << file.first << std::endl;
            ss << file.second << std::endl;
            ss << "#!END" << std::endl;
        }

        ss << std::endl;

        ss << "#!FILE: makefile" << std::endl << makefile << std::endl << "#!END" << std::endl;
        ss << std::endl;

        ss << "#!FILE: bash" << std::endl << bash << std::endl << "#!END" << std::endl;
        ss << std::endl;

        return ss.str();
    }

    ProjectSrc ProjectTemplate::compile(const std::string& name, project::build b, bool fast){
        //Create ProjectSrc

        std::vector<std::pair<std::string, std::string>> src;
        std::vector<std::string> dir;

        std::string prefix = name + "/";

        //Varibles
        std::string includes = (!fast)?"-I ../includes":"";
        std::string c_flags = "-Wall";
        std::string cxx_flags = "-Wall";
        std::string ld_flags = "";

        //For each file replase predefined varibles and push it as src file
        for(auto file: files){
            std::string _name = prefix + file.first;
            std::string _cnt = file.second;

            common::str_replase_all(_name, "#!NAME!#", name);

            common::str_replase_all(_cnt, "#!INCLUDES!#", includes);
            common::str_replase_all(_cnt, "#!C_FLAGS!#", c_flags);
            common::str_replase_all(_cnt, "#!CXX_FLAGS!#", cxx_flags);
            common::str_replase_all(_cnt, "#!LD_FLAGS!#", ld_flags);
            common::str_replase_all(_cnt, "#!NAME!#", name);
            common::str_replase_all(_cnt, "    ", "\t");

            src.emplace_back(_name, _cnt);
        }

        //Choose and prepare build file, add it to src
        std::string build_name = prefix;
        std::string build_cnt;
        switch(b){
            case project::PROJECT_BUILD_MAKE:
                build_name += "makefile";
                build_cnt = makefile;
                break;
            case project::PROJECT_BUILD_BASH:
                if(fast)
                    build_name += ".scripts/make.sh";
                else
                    build_name += "make.sh";
                build_cnt = bash;
                break;
        }

        common::str_replase_all(build_cnt, "#!NAME!#", name);
        common::str_replase_all(build_cnt, "#!INCLUDES!#", includes);
        common::str_replase_all(build_cnt, "#!C_FLAGS!#", c_flags);
        common::str_replase_all(build_cnt, "#!CXX_FLAGS!#", cxx_flags);
        common::str_replase_all(build_cnt, "#!LD_FLAGS!#", ld_flags);
        common::str_replase_all(build_cnt, "    ", "\t");
        
        src.emplace_back(build_name, build_cnt);

        //Prepare externs file
        if(!fast){
            std::string externs_cnt;

            for(auto& e: externs)
                externs_cnt += e.first + " " + e.second + "\n";
            
            src.emplace_back(".externs", externs_cnt);
        }

        //Prepare dirs
        if(!fast)
            dir.push_back(name);

        for(auto& d: directories)
            dir.push_back(prefix + d);
        
        return {src, dir}; //Return new ProjectSrc
    }

    std::vector<ProjectTemplate> load_project_templates(const std::string& p){
        //Loads templates from multi-definition file (or something like this)

        std::vector<ProjectTemplate> templates;

        //Open templates file
        std::ifstream file(p); //p = ".project/templates"
        if(!file.good())
            dtk::log::error("Failed to open file.", 111); //EACCES

        //Read the file line-by-line
        std::string line;
        while(getline(file, line)){
            if(line.empty() || !line.starts_with("#")) //Every line witch don't start by '#' is a comment
                continue;
            
            if(line.starts_with("#Begin")){ //If starts by '#Begin' begin loading new ProjectTemplate to '#End'
                ProjectTemplate p;
                p.load(file, "End");
                templates.push_back(p);
            } else{
                dtk::log::error("Multi-template file internal error: Unknown \"" + line + "\".", 83); //ELOAD
            }
        }
        
        //Close the file
        file.close();

        return templates;
    }

    void str_cleanBegining(std::string& s){
        //remove spacies and tabs from line begining
        int x = 0;

        while(s[x] == ' ' || s[x] == '\t')
            x++;
            
        s = s.substr(x);
    }

    std::vector<ProjectTemplate2> ProjectTemplate2::load(const std::string& p){
        std::vector<ProjectTemplate2> t;

        std::ifstream file(p);
        if(!file.good())
            dtk::log::error("Failed to open file.", 111); //EACCES
        
        std::string line;
        while(getline(file, line)){
            str_cleanBegining(line);

            if(line.empty())
                continue; //nothing to do

            if(line[0] == '<'){ //Section (error unexcepted section if not <template>)
                if(line.starts_with("<template>")){ //Load template
                    ProjectTemplate2 tmpl;

                    bool name_set = false;
                    bool rule_set = false;
                    bool link_set = false;
                    bool template_end = false;

                    while(getline(file, line)){
                        str_cleanBegining(line);

                        if(line.empty())
                            continue; //nothing to do
                        
                        if(line[0] == '<'){ //Load section
                            //Switch section
                            if(line.starts_with("</template>")){ //End of template section
                                template_end = true;
                                break;
                            } else if(line.starts_with("<file>")){ //Load file
                                ProjectTemplateFile2 f;

                                bool f_name_set = false;
                                bool f_cnt_set = false;
                                bool file_end = false;

                                while(getline(file, line)){
                                    str_cleanBegining(line);

                                    if(line.empty())
                                        continue; //nothing to do
                                    
                                    if(line[0] == '<'){ //Load section
                                        if(line.starts_with("</file>")){
                                            file_end = true;
                                            break;
                                        } else if(line.starts_with("<cnt>")){ //Load cnt
                                            std::stringstream cnt;

                                            bool cnt_end = false;

                                            while(getline(file, line)){
                                                str_cleanBegining(line);

                                                if(line.empty())
                                                    continue; //Nothing to do
                                                
                                                if(line.starts_with("</cnt>")){
                                                    cnt_end = true;
                                                    break;
                                                } else if(line[0] == '\''){
                                                    cnt << line.substr(1) << std::endl;
                                                }
                                            }

                                            //Validate
                                            if(!cnt_end)
                                                dtk::log::error("Template internal error: Missing <cnt> section end.", 83); //ELOAD
                                            
                                            //Validation succes, set cnt value and flag
                                            f_cnt_set = true;
                                            f.cnt = cnt.str();
                                        } else{
                                            dtk::log::error("Template internal error: Unknown section name.", 83); //ELOAD
                                        }
                                    } else if(line[0] == '!'){ //Load attribute
                                        //Get attribute name and value
                                        auto colon = line.find(':');
                                        if(colon == std::string::npos)
                                            dtk::log::error("Template internal error: Attribute syntax error - colon not found.", 83); //ELOAD

                                        std::string attribute = line.substr(1, colon - 1);
                                        std::string value = line.substr(colon + 1);
                                        str_cleanBegining(value);

                                        //Switch attribute
                                        if(attribute == "name"){
                                            if(f_name_set)
                                                dtk::log::error("Template internal error: Duplicated name.", 83); //ELOAD
                                
                                            f_name_set = true;
                                            f.name = value;
                                        } else{
                                            dtk::log::error("Template internal error: Unknown attribute.", 83); //ELOAD
                                        }
                                    }
                                }

                                //Validate

                                if(!file_end)
                                    dtk::log::error("Template internal error: Missing <file> section end.", 83); //ELOAD

                                if(!f_name_set)
                                    dtk::log::error("Template internal error: Missing <file> name attribute.", 83); //ELOAD
                                
                                if(!f_cnt_set)
                                    dtk::log::error("Template file internal error: Missing <cnt>.", 83); //ELOAD
                                
                                //Validation succes, add file
                                tmpl.files.push_back(f);
                            } else if(line.starts_with("<rule>")){ //Load rule
                                ProjectTemplateRule2 r;

                                bool in_set = false;
                                bool out_set = false;
                                bool exe_set = false;
                                bool rule_end = false;

                                while(getline(file, line)){
                                    str_cleanBegining(line);

                                    if(line.empty())
                                        continue; //nothing to do

                                    if(line[0] == '<'){ //Error unexcepted section or </rule>
                                        if(line.starts_with("</rule>")){
                                            rule_end = true;
                                            break;
                                        }

                                        dtk::log::error("Template internal error: Unexcepted section.", 83); //ELOAD
                                    } else if(line[0] == '!'){ //Load attribute
                                        //Get attribute name and value
                                        auto colon = line.find(':');
                                        if(colon == std::string::npos)
                                            dtk::log::error("Template internal error: Attribute syntax error - colon not found.", 83); //ELOAD

                                        std::string attribute = line.substr(1, colon - 1);
                                        std::string value = line.substr(colon + 1);
                                        str_cleanBegining(value);

                                        //Switch attribute
                                        if(attribute == "in"){
                                            if(in_set)
                                                dtk::log::error("Template internal error: Rule duplicated in.", 83); //ELOAD

                                            in_set = true;
                                            r.in = value;
                                        } else if(attribute == "out"){
                                            if(out_set)
                                                dtk::log::error("Template internal error: Rule duplicated out.", 83); //ELOAD

                                            out_set = true;
                                            r.out = value;
                                        } else if(attribute == "exe"){
                                            exe_set = true;
                                            r.exes.push_back(value);
                                        } else{
                                            dtk::log::error("Template internal error: Unknown attribute.", 83); //ELOAD
                                        }
                                    }
                                }

                                //Validate

                                if(!rule_end)
                                    dtk::log::error("Template internal error: Missing <rule> section end.", 83); //ELOAD
                                
                                if(!in_set)
                                    dtk::log::error("Template internal error: Missing <rule> in attribute.", 83); //ELOAD
                                
                                if(!out_set)
                                    dtk::log::error("Template internal error: Missing <rule> out attribute.", 83); //ELOAD
                                
                                if(!exe_set)
                                    dtk::log::error("Template internal error: Missing <rule> exe attribute.", 83); //ELOAD

                                //Validation succes, add rule
                                rule_set = true;
                                tmpl.rules.push_back(r);
                            } else if(line.starts_with("<link>")){ //Load link
                                ProjectTemplateRule2 r;

                                bool in_set = false;
                                bool out_set = false;
                                bool exe_set = false;
                                bool link_end = false;

                                while(getline(file, line)){
                                    str_cleanBegining(line);

                                    if(line.empty())
                                        continue; //nothing to do

                                    if(line[0] == '<'){ //Error unexcepted section or </rule>
                                        if(line.starts_with("</link>")){
                                            link_end = true;
                                            break;
                                        }

                                        dtk::log::error("Template internal error: Unexcepted section.", 83); //ELOAD
                                    } else if(line[0] == '!'){ //Load attribute
                                        //Get attribute name and value
                                        auto colon = line.find(':');
                                        if(colon == std::string::npos)
                                            dtk::log::error("Template internal error: Attribute syntax error - colon not found.", 83); //ELOAD

                                        std::string attribute = line.substr(1, colon - 1);
                                        std::string value = line.substr(colon + 1);
                                        str_cleanBegining(value);

                                        //Switch attribute
                                        if(attribute == "in"){
                                            if(in_set)
                                                dtk::log::error("Template internal error: Rule duplicated in.", 83); //ELOAD

                                            in_set = true;
                                            r.in = value;
                                        } else if(attribute == "out"){
                                            if(out_set)
                                                dtk::log::error("Template internal error: Rule duplicated out.", 83); //ELOAD

                                            out_set = true;
                                            r.out = value;
                                        } else if(attribute == "exe"){
                                            exe_set = true;
                                            r.exes.push_back(value);
                                        } else{
                                            dtk::log::error("Template internal error: Unknown attribute.", 83); //ELOAD
                                        }
                                    }
                                }

                                //Validate

                                if(!link_end)
                                    dtk::log::error("Template internal error: Missing <link> section end.", 83); //ELOAD
                                
                                if(!in_set)
                                    dtk::log::error("Template internal error: Missing <link> in attribute.", 83); //ELOAD
                                
                                if(!out_set)
                                    dtk::log::error("Template internal error: Missing <link> out attribute.", 83); //ELOAD
                                
                                if(!exe_set)
                                    dtk::log::error("Template internal error: Missing <link> exe attribute.", 83); //ELOAD

                                //Validation succes, add rule
                                link_set = true;
                                tmpl.links.push_back(r);
                            } else if(line.starts_with("<clean>")){ //Load clean
                                bool clean_end = false;

                                while(getline(file, line)){
                                    str_cleanBegining(line);

                                    if(line.empty())
                                        continue; //nothing to do

                                    if(line[0] == '<'){ //Error unexcepted section or </clean>
                                        if(line.starts_with("</clean>")){
                                            clean_end = true;
                                            break;
                                        }

                                        dtk::log::error("Template internal error: Unexcepted section.", 83); //ELOAD
                                    } else if(line[0] == '!'){ //Load attribute
                                        //Get attribute name and value
                                        auto colon = line.find(':');
                                        if(colon == std::string::npos)
                                            dtk::log::error("Template internal error: Attribute syntax error - colon not found.", 83); //ELOAD

                                        std::string attribute = line.substr(1, colon - 1);
                                        std::string value = line.substr(colon + 1);
                                        str_cleanBegining(value);

                                        //Switch attribute
                                        if(attribute == "dir"){
                                            tmpl.clean.dirs.push_back(value);
                                        } else if(attribute == "file"){
                                            tmpl.clean.files.push_back(value);
                                        } else if(attribute == "exe"){
                                            tmpl.clean.exes.push_back(value);
                                        } else{
                                            dtk::log::error("Template internal error: Unknown attribute.", 83); //ELOAD
                                        }
                                    }
                                }

                                //Validate

                                if(!clean_end)
                                    dtk::log::error("Template internal error: Missing <clean> section end.", 83); //ELOAD
                            } else{
                                dtk::log::error("Template internal error: Unknown section name.", 83); //ELOAD
                            }
                        } else if(line[0] == '!'){ //Load attribute
                            //Get attribute name and value
                            auto colon = line.find(':');
                            if(colon == std::string::npos)
                                dtk::log::error("Template internal error: Attribute syntax error - colon not found.", 83); //ELOAD

                            std::string attribute = line.substr(1, colon - 1);
                            std::string value = line.substr(colon + 1);
                            str_cleanBegining(value);
                            
                            //Switch attribute
                            if(attribute == "name"){ //Load name
                                if(name_set)
                                    dtk::log::error("Template internal error: Duplicated name.", 83); //ELOAD
                                
                                name_set = true;
                                tmpl.name = value;
                            } else if(attribute == "dir"){ //Load dir
                                tmpl.dirs.push_back(value);
                            } else if(attribute == "ebin"){ //Load export binary
                                tmpl.exports.emplace_back(value, project::PROJECT_EXPORT_BIN);
                            } else if(attribute == "elib"){ //Load export lib
                                tmpl.exports.emplace_back(value, project::PROJECT_EXPORT_LIB);
                            } else if(attribute == "einc"){ //Load export include
                                tmpl.exports.emplace_back(value, project::PROJECT_EXPORT_INCLUDE);
                            } else{
                                dtk::log::error("Template internal error: Unknown attribute.", 83); //ELOAD
                            }
                        }
                    }

                    //Validate

                    if(!template_end)
                        dtk::log::error("Template internal error: Missing <template> section end.", 83); //ELOAD
                    
                    if(!name_set)
                        dtk::log::error("Template internal error: Missing <template> name attribute.", 83); //ELOAD
                    
                    if(!rule_set)
                        dtk::log::error("Template internal error: Missing <rule>.", 83); //ELOAD
                    
                    if(!link_set)
                        dtk::log::error("Template internal error: Missing <link>.", 83); //ELOAD
                    
                    //Validation succes, all template
                    t.push_back(tmpl);
                } else{
                    dtk::log::error("Template internal error: Unexcepted section.", 83); //ELOAD
                }
            } else if(line[0] == '!'){ //Attribute (error unexceptrd attribute)
                dtk::log::error("Template internal error: Unexcepted attribute.", 83); //ELOAD
            }
        }

        file.close();

        return t;
    }

    std::string ProjectTemplate2::to_string() const{
        std::stringstream ss;
        ss << "<template>" << std::endl;
        ss << "\t!name: " << name << std::endl;

        for(auto& d: dirs)
            ss << "\t!dir: " << d << std::endl;
        
        for(auto& e: exports){
            switch(e.type){
                case project::PROJECT_EXPORT_BIN:
                    ss << "\t!ebin: " << e.name << std::endl;
                    break;
                case project::PROJECT_EXPORT_LIB:
                    ss << "\t!elib: " << e.name << std::endl;
                    break;
                case project::PROJECT_EXPORT_INCLUDE:
                    ss << "\t!einc: " << e.name << std::endl;
                    break;
            }
        }

        for(auto& f: files){
            ss << "\t<file>" << std::endl;
            ss << "\t\t!name: " << f.name << std::endl;
            ss << "\t\t<cnt>" << std::endl;

            std::stringstream cnt(f.cnt);
            std::string line;

            while(getline(cnt, line))
                ss << "\t\t\t'" << line << std::endl;
            
            ss << "\t\t</cnt>" << std::endl;
            ss << "\t</file>" << std::endl;
        }

        ss << "\t<clean>" << std::endl;

        for(auto& d: clean.dirs)
            ss << "\t\t!dir: " << d << std::endl;
        
        for(auto& f: clean.files)
            ss << "\t\t!file: " << f << std::endl;

        for(auto& e: clean.exes)
            ss << "\t\t!exe: " << e << std::endl;
        
        ss << "\t</clean>" << std::endl;

        for(auto& r: rules){
            ss << "\t<rule>" << std::endl;
            ss << "\t\t!in: " << r.in << std::endl;
            ss << "\t\t!out: " << r.out << std::endl;

            for(auto& exe: r.exes)
                ss << "\t\t!exe: " << exe << std::endl;
            
            ss << "\t</rule>" << std::endl;
        }

        for(auto& l: links){
            ss << "\t<link>" << std::endl;
            ss << "\t\t!in: " << l.in << std::endl;
            ss << "\t\t!out: " << l.out << std::endl;

            for(auto& exe: l.exes)
                ss << "\t\t!exe: " << exe << std::endl;
            
            ss << "\t</link>" << std::endl;
        }

        ss << "</template>" << std::endl;
        return ss.str();
    }

    void postprocess(std::string& s, const std::string& name, 
                                     const std::string& in, 
                                     const std::string& out, 
                                     const std::string& includes, 
                                     const std::string& libs,
                                     const std::string& star
                    ){
        common::str_replase_all(s, "<$name>", name);

        common::str_replase_all(s, "<$in>", in);
        common::str_replase_all(s, "<$out>", out);
        common::str_replase_all(s, "<$includes>", includes);
        common::str_replase_all(s, "<$libs>", libs);

        common::str_replase_all(s, "<*>", star);

        common::str_replase_all(s, "<$lt>", "<");
        common::str_replase_all(s, "<$gt>", ">");
    }

    ProjectCompiledTemplate2 ProjectTemplate2::compile(const std::string& name, project::build b, bool fast) const{
        ProjectCompiledTemplate2 c;

        //Sending data
        std::string prefix = name + "/";

        for(auto& d: dirs)
            c.dirs.push_back(prefix + d);
        
        for(auto& f: files)
            c.files.push_back(f);
        
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
                postprocess(e, name, "", "$@", "", "", "%");
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

            //Prepare rules
            for(auto& r: rules){
                build << r.out << ": " << r.in << std::endl;
                build << "\t@echo Compiling $^" << std::endl;
                for(auto e: r.exes){
                    postprocess(e, name, "$^", "$@", (!fast)?"-I ../includes":"", (!fast)?"-L ../libs":"", "%");
                    build << "\t@" << e << std::endl;
                }
                build << std::endl;
            }

            //Prepare links
            for(auto& l: links){
                build << l.out << ": " << l.in << std::endl;
                build << "\t@echo Linking $@" << std::endl;
                for(auto e: l.exes){
                    postprocess(e, name, "$^", "$@", (!fast)?"-I ../includes":"", (!fast)?"-L ../libs":"", "%");
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
                postprocess(e, name, "", "clean", "", "", "*");
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
            build << std::endl;
            for(auto& r: rules){
                build << "for file in " << r.in << std::endl;
                build << "do" << std::endl;
                build << "\techo Compiling $file" << std::endl;

                //Calculating out varible
                auto out = r.out;
                auto star = r.in.find("<*>");
                common::str_replase_all(out, "<*>", std::string("${file#\"") + r.in.substr(0, star) + "\"};out=${out%\"" + r.in.substr(star + 3) + "\"}");
                build << "\tout=" << out << std::endl;

                for(auto e: r.exes){
                    postprocess(e, name, "$file", "$out", (!fast)?"-I ../includes":"", (!fast)?"-L ../libs":"", "*");
                    build << "\t" << e << std::endl;
                }
                build << "done" << std::endl;
                build << std::endl;
            }

            //Prepare links
            build << "#Links:" << std::endl;
            build << std::endl;
            for(auto& l: links){
                build << "echo Linking " << l.out << std::endl;
                for(auto e: l.exes){
                    postprocess(e, name, "$file", l.out, (!fast)?"-I ../includes":"", (!fast)?"-L ../libs":"", "*");
                    build << e << std::endl;
                }
                build << std::endl;
            }
        }

        c.files.emplace_back(prefix + build_name, build.str());

        //Posprocess

        for(auto& d: c.dirs)
            postprocess(d, name, "", "", "", "", (b == project::PROJECT_BUILD_MAKE)?"%":"*");

        for(auto& f: c.files){
            postprocess(f.name, name, "", "", "", "", (b == project::PROJECT_BUILD_MAKE)?"%":"*");
            postprocess(f.cnt, name, "", "", "", "", (b == project::PROJECT_BUILD_MAKE)?"%":"*");
        }

        //Copping exports
        c.exports = exports;

        return c;
    }
}