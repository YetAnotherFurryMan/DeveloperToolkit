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

    void str_cleanBegining(std::string& s){
        //remove spacies and tabs from line begining
        int x = 0;

        while(s[x] == ' ' || s[x] == '\t')
            x++;
            
        s = s.substr(x);
    }

    std::vector<ProjectTemplate> ProjectTemplate::load(const std::string& p){
        std::vector<ProjectTemplate> t;

        std::ifstream file(p);
        if(!file.good())
            dtk::log::error("Failed to open template file.", 111); //EACCES
        
        std::string line;
        while(getline(file, line)){
            str_cleanBegining(line);

            if(line.empty())
                continue; //nothing to do

            if(line[0] == '<'){ //Section (error unexcepted section if not <template>)
                if(line.starts_with("<template>")){ //Load template
                    ProjectTemplate tmpl;

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
                                ProjectTemplateFile f;

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
                                ProjectTemplateRule r;

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
                                ProjectTemplateRule r;

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

    std::string ProjectTemplate::to_string() const{
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

    void inpostprocess(std::string& s, const std::string& in,
                                     const std::string& out,
                                     const std::string& star
                    ){
        common::str_replase_all(s, "<$in>", in);
        common::str_replase_all(s, "<$out>", out);

        common::str_replase_all(s, "<*>", star);
    }

    void postprocess(std::string& s, const std::string& name,
                                     const std::string& includes, 
                                     const std::string& libs,
                                     const std::string& build
                    ){
        common::str_replase_all(s, "<$name>", name);

        common::str_replase_all(s, "<$includes>", includes);
        common::str_replase_all(s, "<$libs>", libs);
        common::str_replase_all(s, "<$build>", build);

        common::str_replase_all(s, "<$lt>", "<");
        common::str_replase_all(s, "<$gt>", ">");
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
                common::str_replase_all(out, "<*>", std::string("${file#\"") + r.in.substr(0, star) + "\"};out=${out%\"" + r.in.substr(star + 3) + "\"}");
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
}