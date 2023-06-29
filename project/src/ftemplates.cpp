#include <ftemplates.hpp>

#include <filesystem>
#include <fstream>

#include <macros.hpp>

namespace ftemplates{
    void create_project(const std::string& name, project::type t, project::build b, const char* project_file, const char* templates_file, const char* modules_file){
        bool fast = t == project::PROJECT_TYPE_SOLUTION;

        //Create project directories
        CREATE_DIR(name);
        CREATE_DIR(name + "/.project");
        CREATE_DIR(name + "/.scripts");

        if(!fast){
            CREATE_DIR(name + "/res");
            CREATE_DIR(name + "/include");
            CREATE_DIR(name + "/lib");
            CREATE_DIR(name + "/bin");
        }

        //Create project file
        std::ofstream project(name + "/" + project_file);
        FILE_OK(project);
        
        project << "#Name: " << name << std::endl;
        project << "#Type: " << t << std::endl;
        project << "#Build: " << b << std::endl;
        
        project.close();

        //Create the templates file (with no data)
        CREATE_FILE(name + "/" + templates_file);

        //Create scripts (and the modules file in non-fast case)
        if(!fast){
            CREATE_FILE(name + "/" + modules_file);

            CREATE_SCRIPT(name + "/.scripts/clean.sh");
            CREATE_SCRIPT(name + "/.scripts/make.sh");
            CREATE_SCRIPT(name + "/.scripts/test.sh");
        } else{
            std::ofstream script_test(name + "/.scripts/test.sh");
            FILE_OK(script_test);

            script_test << "#!/bin/bash" << std::endl;
            script_test << std::endl;

            switch(b){
                case project::PROJECT_BUILD_MAKE:
                    script_test << "make test" << std::endl;
                    script_test << std::endl;
                    break;
                case project::PROJECT_BUILD_BASH:
                    script_test << "./" << name << std::endl;
                    break;
            }

            script_test.close();
        }

        //Create run script
        update_run_script(name + "/run", b, fast);
    }

    void update_run_script(const std::string& p, project::build b, bool fast, unsigned int enabled){
        bool log = enabled & project::PROJECT_ENABLE_LOG;
        bool tests = enabled & project::PROJECT_ENABLE_TESTS;

        //Open run script
        std::ofstream run(p);// name + "/run"
        FILE_OK(run);

        //Get make command
        std::string make_cmd = "bash .scripts/make.sh";
        if(fast && b != project::PROJECT_BUILD_BASH)
            make_cmd = "make";
        
        //Begin as bash script
        run << "#!/bin/bash" << std::endl;
        run << std::endl;

        if(log){ //log case
            run << "#Get the log file name" << std::endl;
            run << "log=$(date +'log/log%Y.%j.txt')" << std::endl;
            run << "mkdir log 2>/dev/null" << std::endl;
            run << std::endl;
            run << "echo $(date +'#Date: %F %T:%N') >> $log" << std::endl;
            run << "echo \"\" >> $log" << std::endl;
            run << std::endl;

            if(!fast){
                run << "#Clean" << std::endl;
                run << "echo \"#Clean:\" >> $log" << std::endl;
                run << "bash .scripts/clean.sh >> $log" << std::endl;
                run << "echo \"\" >> $log" << std::endl;
                run << std::endl;
            }

            run << "#Make" << std::endl;
            run << "echo \"#Make:\" >> $log" << std::endl;
            run << make_cmd << " >> $log" << std::endl;
            run << "echo \"\" >> $log" << std::endl;
            run << std::endl;

            if(tests){
                run << "#Test" << std::endl;
                run << "echo \"#Test:\" >> $log" << std::endl;
                run << "bash .scripts/test.sh >> $log" << std::endl;
                run << "echo \"\" >> $log" << std::endl;
            }
        } else{ //normal case
            if(!fast){
                run << "#Clean" << std::endl;
                run << "bash .scripts/clean.sh" << std::endl;
                run << std::endl;
            }

            run << "#Make" << std::endl;
            run << make_cmd << std::endl;
            run << std::endl;

            if(tests){
                run << "#Test" << std::endl;
                run << "bash .scripts/test.sh" << std::endl;
            }
        }

        //Close the file
        run.close();
    }

    void update_make_script(const std::string& p, const fdata::ProjectModulesFile& modules, project::build b){
        //Open make.sh
        std::ofstream make(p);
        FILE_OK(make);

        make << "#!/bin/bash" << std::endl;
        make << std::endl;

        //Clean
        make << "#Clean" << std::endl;
        make << "rm -fr bin/* 2> /dev/null" << std::endl;
        make << "rm -fr lib/* 2> /dev/null" << std::endl;
        make << "rm -fr includes/* 2> /dev/null" << std::endl;
        make << "rm -fr res/* 2> /dev/null" << std::endl;
        make << std::endl;

        //Modules
        for(auto& m: modules.modules){
            make << "#" << m.name << std::endl;

            //Make module
            switch(b){
                case project::PROJECT_BUILD_MAKE:
                    make << "make -C " << m.name << std::endl;
                    break;
                case project::PROJECT_BUILD_BASH:
                    make << "cd " << m.name << std::endl;
                    make << "bash make.sh" << std::endl;
                    make << "cd .." << std::endl;
                    break;
            }

            //Export files
            for(auto& e: m.exports){
                make << "cp " << m.name << "/" << e.name;

                switch(e.type){
                    case project::PROJECT_EXPORT_BIN:
                        make << " bin/";
                        break;
                    case project::PROJECT_EXPORT_LIB:
                        make << " lib/";
                        break;
                    case project::PROJECT_EXPORT_INCLUDE:
                        make << " includes/";
                        break;
                    case project::PROJECT_EXPORT_RES:
                        make << " res/";
                        break;
                }

                make << e.name << std::endl;
            }
        }

        make.close();
    }

    void update_templates_file(const std::string& p, const std::vector<fdata::ProjectTemplate>& templates){
        //Get already added templates
        auto added = fdata::ProjectTemplate::load(p);
        
        //Strore seen names
        std::vector<std::string> names;

        //Open temlates file
        std::ofstream templates_file(p);
        FILE_OK(templates_file);

        //Add already known templates and store its names (validate them)
        for(auto& tmpl: added){
            bool found = false;

            for(auto& name: names){
                if(name == tmpl.name){
                    dtk::log::error("Template \"" + name + "\" already added.");
                    found = true;
                    break;
                }
            }

            if(!found){
                names.push_back(tmpl.name);

                templates_file << "<!-- Temaplate: " << tmpl.name << " -->" << std::endl;
                templates_file << tmpl.to_string() << std::endl;
            }
        }

        //Do the same for new templates
        for(auto& tmpl: templates){
            bool found = false;

            for(auto& name: names){
                if(name == tmpl.name){
                    dtk::log::error("Template \"" + name + "\" already added.");
                    found = true;
                    break;
                }
            }

            if(!found){
                names.push_back(tmpl.name);

                templates_file << "<!-- Template: " << tmpl.name << " -->" << std::endl;
                templates_file << tmpl.to_string() << std::endl;
            }
        }

        //Close the file
        templates_file.close();
    }

    void build_template(const fdata::ProjectCompiledTemplate& t){
        //Created directories
        for(auto& d: t.dirs)
            CREATE_DIR(d);
        
        //Create files
        for(auto& f: t.files){
            std::ofstream file(f.name);
            FILE_OK(file);

            file << f.cnt;

            file.close();
        }
    }
}