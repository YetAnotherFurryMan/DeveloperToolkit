#include <iostream>
#include <fstream>

#include <cstring>

//#define STRCMP_EQ(a, b) std::strcmp(a, b) == 0

#include <log.hpp>

#include <defs.hpp>
#include <macros.hpp>
#include <fdata.hpp>
#include <ftemplates.hpp>

namespace pr = project;
namespace fd = fdata;
namespace ft = ftemplates;

int main(int argc, char** argv){
    //Skipp execution
    argc--; argv++;

    if(argc == 0)
        dtk::log::fatal_error("Excepted arguments.", 130); //ENOEXEC

    //Choose task
    char* task = argv[0]; argc--; argv++; //Get a task name and pop the argument
    if(STRCMP_EQ(task, "init")){
        std::string name;
        std::string templ;
        std::vector<std::string> templs;
        pr::type type = pr::PROJECT_TYPE_PROJECT;
        pr::build build = pr::PROJECT_BUILD_MAKE;

        bool build_set = false;

        //Load properties
        for(int i = 0; i < argc; i++){
            if(STRCMP_EQ(argv[i], "-n") || STRCMP_EQ(argv[i], "--name")){ //Load a name, return an error if already exists or there is a syntax error
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-n'.", 83); //ELOAD
                
                if(!name.empty())
                    dtk::log::fatal_error("Name already has been set.");
                
                name = argv[i];
            } else if(STRCMP_EQ(argv[i], "-b") || STRCMP_EQ(argv[i], "--build")){ //Load a build system, return an error if was already seted or there is a syntax error
                if(build_set)
                    dtk::log::fatal_error("Project build system already has been set.");
                
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-b'.", 83); //ELOAD
                
                if(STRCMP_EQ(argv[i], "make") || STRCMP_EQ(argv[i], "makefile")) //Set to MAKE
                    build = pr::PROJECT_BUILD_MAKE;
                else if(STRCMP_EQ(argv[i], "bash")) //Set to BASH
                    build = pr::PROJECT_BUILD_BASH;
                else
                    dtk::log::fatal_error("Unsuported build system.");
                
                build_set = true;
            } else if(STRCMP_EQ(argv[i], "-t") || STRCMP_EQ(argv[i], "--template")){ //Load a template name, return an error if was already seted or there is a syntax error, set project to fast
                if(!templ.empty())
                    dtk::log::fatal_error("Project template already has been set.");
                
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-t'.", 83); //ELOAD
                
                templ = argv[i];
                type = pr::PROJECT_TYPE_SOLUTION;
            } else if(STRCMP_EQ(argv[i], "-T") || STRCMP_EQ(argv[i], "--template-file")){ //Add a template file, return an error if there is a syntax error
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-T'.", 83); //ELOAD
                
                templs.emplace_back(argv[i]);
            } else{
                dtk::log::fatal_error(std::string("Unknown argument \"") + argv[i] + "\"", 158); //EMVSPARM
            }
        }

        //Load templates
        std::vector<fd::ProjectTemplate> templates = fd::ProjectTemplate::load("templates/default.txt");

        for(auto& tp: templs){
            auto tmpls = fd::ProjectTemplate::load(tp);
            
            for(auto& tmpl: tmpls){
                bool found = false;
                for(auto& t: templates){
                    if(t.name == tmpl.name){
                        found = true;
                        break;
                    }
                }

                if(!found)
                    templates.push_back(tmpl);
            }
        }

        //Create the project files and scripts
        ft::create_project(name, type, build);

        ft::update_templates_file(name + "/.project/templates", templates);

        if(type == pr::PROJECT_TYPE_SOLUTION){ //Use template if project is fast
            fd::ProjectTemplate& used_templ = templates[0];
            
            //Find choosen template and return an error if not found
            bool found = false;
            for(auto& t: templates){
                if(t.name == templ){
                    used_templ = t;
                    found = true;
                    break;
                }
            }

            if(!found)
                dtk::log::fatal_error("Template not found \"" + templ + "\".", 83); //ELOAD

            //Build files
            ft::build_template(used_templ.compile(name, build, true));
        }
    } else if(STRCMP_EQ(task, "info")){
        //Load project data, validate and output them
        fd::ProjectFile project("./.project/project");
        
        dtk::log::info("Name: " + project.name);
        
        switch(project.type){
            case pr::PROJECT_TYPE_PROJECT:
                dtk::log::info("Type: PROJECT");
                break;
            case pr::PROJECT_TYPE_SOLUTION:
                dtk::log::info("Type: SOLUTION");
                break;
        }

        if(project.type != pr::PROJECT_TYPE_PROJECT)
            dtk::log::warning("Project is non-modular.");
        
        switch(project.build){
            case pr::PROJECT_BUILD_MAKE:
                dtk::log::info("Build system: MAKE");
                break;
            case pr::PROJECT_BUILD_BASH:
                dtk::log::info("Build system: BASH");
                break;
        }

        if(project.enabled){
            std::string enabled = "";

            if(project.enabled & pr::PROJECT_ENABLE_LOG)
                enabled += "\"silent\" ";
            if(project.enabled & pr::PROJECT_ENABLE_TESTS)
                enabled += "\"tests\" ";

            dtk::log::info("Enabled: " + enabled);
        }
    } else if(STRCMP_EQ(task, "enable")){
        //Load project data and try to enable the functionality
        fd::ProjectFile project("./.project/project");
        bool fast = project.type == pr::PROJECT_TYPE_SOLUTION;

        //Get functionality and pop it
        if(argc <= 0)
            dtk::log::fatal_error("Excepted the functionality name.", 83); //ELOAD
        
        char* functionality = argv[0];
        argv++;argc--;

        //Select the functionality and enable it (if hadn't been yet)
        if(STRCMP_EQ(functionality, "log") || STRCMP_EQ(functionality, "silent")){
            if(project.enabled & pr::PROJECT_ENABLE_LOG){
                dtk::log::warning("\"silent\" already enabled.");
                return 0;
            }

            project.enabled |= pr::PROJECT_ENABLE_LOG;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else if(STRCMP_EQ(functionality, "test") || STRCMP_EQ(functionality, "tests")){
            if(project.enabled & pr::PROJECT_ENABLE_TESTS){
                dtk::log::warning("\"tests\" already enabled.");
                return 0;
            }

            project.enabled |= pr::PROJECT_ENABLE_TESTS;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else{
            dtk::log::fatal_error(std::string("Invalid functionality name\"") + functionality + "\"", 130); //ENOEXEC
        }

        project.update();
    } else if(STRCMP_EQ(task, "disable")){
        //Load project data and try to disable the functionality
        fd::ProjectFile project("./.project/project");
        bool fast = project.type == pr::PROJECT_TYPE_SOLUTION;

        //Get functionality and pop it
        if(argc <= 0)
            dtk::log::fatal_error("Excepted the functionality name.", 83); //ELOAD
        
        char* functionality = argv[0];
        argv++;argc--;

        //Select the functionality and disable it (if hadn't been yet)
        if(STRCMP_EQ(functionality, "log")  || STRCMP_EQ(functionality, "silent")){
            if(project.enabled & pr::PROJECT_ENABLE_LOG == 0){
                dtk::log::warning("\"silent\" already disabled.");
                return 0;
            }

            project.enabled &= ~pr::PROJECT_ENABLE_LOG;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else if(STRCMP_EQ(functionality, "test") || STRCMP_EQ(functionality, "tests")){
            if(project.enabled & pr::PROJECT_ENABLE_TESTS == 0){
                dtk::log::warning("\"tests\" already disabled.");
                return 0;
            }

            project.enabled &= ~pr::PROJECT_ENABLE_TESTS;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else{
            dtk::log::fatal_error(std::string("Invalid functionality name\"") + functionality + "\"", 130); //ENOEXEC
        }

        //Save changes
        project.update();
    } else if(STRCMP_EQ(task, "template")){
        std::vector<fd::ProjectTemplate> templates;

        //Load args
        for(int i = 0; i < argc; i++){
            if(STRCMP_EQ(argv[i], "-T") || STRCMP_EQ(argv[i], "--template-file")){ //Add a template, return an error if there is a syntax error
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-T'.", 83); //ELOAD

                auto tmpls = fd::ProjectTemplate::load(argv[i]); //Load templates

                for(auto& tmpl: tmpls)
                    templates.push_back(tmpl);
            } else{
                dtk::log::fatal_error(std::string("Unknown argument \"") + argv[i] + "\"", 158); //EMVSPARM
            }
        }

        ft::update_templates_file(".project/templates", templates);
    } else if(STRCMP_EQ(task, "module")){
        //Load project
        fd::ProjectFile project(".project/project");

        //Load data
        std::string name;
        std::string tmpl;

        bool name_set = false;
        bool tmpl_set = false;

        //Load args
        for(int i = 0; i < argc; i++){
            if(STRCMP_EQ(argv[i], "-t") || STRCMP_EQ(argv[i], "--template")){ //Set template, return error if already setted
                if(tmpl_set)
                    dtk::log::fatal_error("Template already setted.", 83); //ELOAD

                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-t'.", 83); //ELOAD

                tmpl = argv[i];
                tmpl_set = true;
            } else if(STRCMP_EQ(argv[i], "-n") || STRCMP_EQ(argv[i], "--name")){ //Set name
                if(name_set)
                    dtk::log::fatal_error("Name already setted.", 83); //ELOAD

                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '-n'.", 83); //ELOAD

                name = argv[i];
                name_set = true;
            } else{
                dtk::log::fatal_error(std::string("Unknown argument \"") + argv[i] + "\"", 158); //EMVSPARM
            }
        }

        //Validate data
        if(!name_set)
            dtk::log::fatal_error("Name has not been specified.", 83); //ELOAD
        
        if(!tmpl_set)
            dtk::log::fatal_error("Template has not been specified.", 83); //ELOAD
        
        //Load templates
        auto templates = fd::ProjectTemplate::load(".project/templates");

        //Choose template
        fd::ProjectTemplate& used_templ = templates[0];
            
        //Find template and return an error if not found
        bool found = false;
        for(auto& t: templates){
            if(t.name == tmpl){
                used_templ = t;
                found = true;
                break;
            }
        }

        if(!found)
            dtk::log::fatal_error("Template not found \"" + tmpl + "\".", 83); //ELOAD

        //Build module
        ft::build_template(used_templ.compile(name, project.build));

        //Add module to project
        fd::ProjectModulesFile modules(".project/modules");
        modules.modules.push_back(used_templ.get_module(name, project.build));
        modules.update();

        //Update make.sh
        ft::update_make_script(".scripts/make.sh", modules, project.build);
    } else{
        dtk::log::fatal_error(std::string("Invalid task name \"") + task + "\"", 130); //ENOEXEC
    }
    return 0;
}