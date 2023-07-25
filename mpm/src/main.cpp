#include <iostream>
#include <fstream>

#include <cstring>

#include <log.hpp>

#define DTK_ML_DTK_BUILDER
#define DTK_ML_STR_BUILDER

#include <defs.hpp>
#include <macros.hpp>
#include <fdata.hpp>
#include <ftemplates.hpp>

namespace pr = project;
namespace fd = fdata;
namespace ft = ftemplates;

const char* help =  "The project's help page:\n" \
                    "\n" \
                    "Usage:\n" \
                    "\tproject [TASK] {args}\n" \
                    "\n" \
                    "Tasks:\n" \
                    "1. init\n" \
                    "\tUsage (1):\n" \
                    "\t\tproject init -n [NAME] {-T [TEMPLATE_FILE]}\n" \
                    "\tDescription:\n" \
                    "\t\tInitializes a new modular project in a new directory with the same name.\n" \
                    "\t\tOptional adds a templates' collection file(s) if given ('-T').\n" \
                    "\tUsage (2):\n" \
                    "\t\tproject init -n [NAME] -t [TEMPLATE] {-T [TEMPLATE_FILE]}\n" \
                    "\tDescription:\n" \
                    "\t\tInitializes a new non-modular project in a new directory with the same name using selected template ('-t').\n" \
                    "\t\tOptional adds a templates' collection file(s) if given ('-T').\n" \
                    "\tWhere:\n" \
                    "\t\t[NAME] is a name of the project and directory witch will be created.\n" \
                    "\t\t[TEMPLATE] is a template name.\n" \
                    "\t\t[TEMPLATE_FILE] is a path to a templates' collection file in DTK format.\n" \
                    "2. add\n" \
                    "\tUsage (1):\n" \
                    "\t\tproject add template {[PATH]} {-n [NAME]}\n" \
                    "\tDescription:\n" \
                    "\t\tAdds a templates' collection file(s) from [PATH].\n" \
                    "\t\tIf any '-n' given only templates with given [NAME] will be added.\n" \
                    "\tUsage (2):\n" \
                    "\t\tproject add module -n [NAME] -t [TEMPLATE]\n" \
                    "\tDescription:\n" \
                    "\t\tInitializes a new module in the project using selected template.\n" \
                    "\tUsage (3):\n" \
                    "\t\tproject add [WHAT] -v [VALUE] {-m [MODULE]}\n" \
                    "\tDescription:\n" \
                    "\t\tAdds VALUE as WHAT to all modules in the project or to the selected module.\n" \
                    "\tWhere:\n" \
                    "\t\t[NAME] is a name of a template (1), a new module (2).\n" \
                    "\t\t[TEMPLATE] is a template name (1) (2).\n" \
                    "\t\t[PATH] is a path to a templates' collection file in DTK format (1).\n" \
                    "\t\t[WHAT] is one of these: binary (bin), library (lib), include (inc) and resource (res) and with _directory (_dir) (3).\n" \
                    "\t\t[VALUE] is a property value (3).\n" \
                    "\t\t[MODULE] is a module name (3).\n" \
                    "3. enable\n" \
                    "\tUsage:\n" \
                    "\t\tproject enable [WHAT]\n" \
                    "\tDescription:\n" \
                    "\t\tEnables [WHAT] in the project. The project ('.project/project') file will be recreated.\n" \
                    "\tWhere:\n" \
                    "\t\t[WHAT] is a name of a feuture to enable (\"tests\", \"silence\").\n" \
                    "4. disable\n" \
                    "\tUsage:\n" \
                    "\t\tproject disable [WHAT]\n" \
                    "\tDescription:\n" \
                    "\t\tDisables [WHAT] in the project. The project ('.project/project') file will be recreated.\n" \
                    "\tWhere:\n" \
                    "\t\t[WHAT] is a name of a feuture to disable (\"tests\", \"silence\").\n" \
                    "5. info\n" \
                    "\tUsage:\n" \
                    "\t\tproject info\n" \
                    "\tDescription:\n" \
                    "\t\tPrints all avaliable information abount the project. If project is modular lists modules and templates.\n" \
                    "6. help\n" \
                    "\tUsage:\n" \
                    "\t\tproject help\n" \
                    "\tDescription:\n" \
                    "\t\tPrints this help and exits.\n" \
                    "\n";

void module_add_import_load_args(std::string& value, std::string& module_name, int argc, char** argv);
void module_add_import(const std::string& val, pr::item_type t, const std::string module_name, const fd::ProjectFile& project);

int main(int argc, char** argv){
    //Skipp execution
    argc--; argv++;

    if(argc == 0){
        std::cout << help << std::endl;
        return 0;
    }

    //Choose task
    char* task = argv[0]; 
    argc--; argv++; //Get a task name and pop the argument
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
                    dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD
                
                if(!name.empty())
                    dtk::log::fatal_error("Name already has been set.", 83); //ELOAD
                
                name = argv[i];
            } else if(STRCMP_EQ(argv[i], "-b") || STRCMP_EQ(argv[i], "--build")){ //Load a build system, return an error if was already seted or there is a syntax error
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD

                if(build_set)
                    dtk::log::fatal_error("Project build system already has been set.", 83); //ELOAD
                
                if(STRCMP_EQ(argv[i], "make") || STRCMP_EQ(argv[i], "makefile")) //Set to MAKE
                    build = pr::PROJECT_BUILD_MAKE;
                else if(STRCMP_EQ(argv[i], "bash")) //Set to BASH
                    build = pr::PROJECT_BUILD_BASH;
                else
                    dtk::log::fatal_error("Unsupported build system.", 83); //ELOAD
                
                build_set = true;
            } else if(STRCMP_EQ(argv[i], "-t") || STRCMP_EQ(argv[i], "--template")){ //Load a template name, return an error if was already seted or there is a syntax error, set project to fast
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD

                if(!templ.empty())
                    dtk::log::fatal_error("Project template already has been set.", 83); //ELOAD
                
                templ = argv[i];
                type = pr::PROJECT_TYPE_SOLUTION;
            } else if(STRCMP_EQ(argv[i], "-T") || STRCMP_EQ(argv[i], "--template-file")){ //Add a template file, return an error if there is a syntax error
                if(++i >= argc)
                    dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD
                
                templs.emplace_back(argv[i]);
            } else{
                dtk::log::fatal_error(std::string("Unknown argument \"") + argv[i] + "\"", 158); //EMVSPARM
            }
        }

        //Validate
        if(name.empty())
            dtk::log::fatal_error("Name not given.", 83); //ELOAD

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

        //Create the project's files and scripts
        if(type == pr::PROJECT_TYPE_SOLUTION){ //Use template if project is fast
            if(templates.empty())
                dtk::log::fatal_error("Template not found \"" + templ + "\".", 83); //ELOAD

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
            ft::create_project(name, type, build, PROJECT_FILE, TEMPLATES_FILE, MODULES_FILE);
            ft::update_templates_file(name + "/" + TEMPLATES_FILE, templates);
            ft::build_template(used_templ.compile(name, build, true));
        } else{
            ft::create_project(name, type, build, PROJECT_FILE, TEMPLATES_FILE, MODULES_FILE);
            ft::update_templates_file(name + "/" + TEMPLATES_FILE, templates);
        }
    } else if(STRCMP_EQ(task, "info")){
        //Load project data, validate and output them
        fd::ProjectFile project(PROJECT_FILE);

        std::cout << "project info:" << std::endl;
        
        std::cout << "Name: " << project.name << std::endl;
        
        switch(project.type){
            case pr::PROJECT_TYPE_PROJECT:
                std::cout << "Type: PROJECT" << std::endl;
                break;
            case pr::PROJECT_TYPE_SOLUTION:
                std::cout << "Type: SOLUTION" << std::endl;
                break;
        }
        
        switch(project.build){
            case pr::PROJECT_BUILD_MAKE:
                std::cout << "Build system: MAKE" << std::endl;
                break;
            case pr::PROJECT_BUILD_BASH:
                std::cout << "Build system: BASH" << std::endl;
                break;
        }

        if(project.enabled){
            std::cout << "Enabled: ";

            if(project.enabled & pr::PROJECT_ENABLE_LOG)
                std::cout << "\"silent\" ";
            if(project.enabled & pr::PROJECT_ENABLE_TESTS)
                std::cout << "\"tests\" ";

            std::cout << std::endl;
        }

        if(project.type == pr::PROJECT_TYPE_SOLUTION){
            dtk::log::warning("Project is non-modular.");
            return 0;
        }

        //Load templates
        std::vector<fd::ProjectTemplate> templates = fd::ProjectTemplate::load(TEMPLATES_FILE);

        if(templates.empty()){
            dtk::log::warning("No templates found.");
        } else{
            std::cout << "Loaded templates: ";

            for(auto& t: templates)
                std::cout << t.name << " ";

            std::cout << std::endl;
        }

        //Load modules
        fd::ProjectModulesFile modules(MODULES_FILE);

        if(modules.modules.empty()){
            dtk::log::warning("No modules found.");
        } else{
            std::cout << "Modules: ";

            for(auto& m: modules.modules)
                std::cout << m.name << "(" << m.tmpl_name << ") ";

            std::cout << std::endl;
        }
    } else if(STRCMP_EQ(task, "enable")){
        //Load project data
        fd::ProjectFile project(PROJECT_FILE);
        bool fast = project.type == pr::PROJECT_TYPE_SOLUTION;

        //Get [WHAT] and pop it
        if(argc <= 0)
            dtk::log::fatal_error("No arguments.", 83); //ELOAD
        
        char* what = argv[0];
        argv++;argc--;

        //Select [WHAT] and enable it (if hadn't been yet)
        if(STRCMP_EQ(what, "log") || STRCMP_EQ(what, "silent")){
            if(project.enabled & pr::PROJECT_ENABLE_LOG){
                dtk::log::warning("\"" + std::string(what) + "\" already enabled.");
                return 0;
            }

            project.enabled |= pr::PROJECT_ENABLE_LOG;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else if(STRCMP_EQ(what, "test") || STRCMP_EQ(what, "tests")){
            if(project.enabled & pr::PROJECT_ENABLE_TESTS){
                dtk::log::warning("\"" + std::string(what) + "\" already enabled.");
                return 0;
            }

            project.enabled |= pr::PROJECT_ENABLE_TESTS;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else{
            dtk::log::fatal_error("Invalid functionality name\"" + std::string(what) + "\"", 130); //ENOEXEC
        }

        project.update();
    } else if(STRCMP_EQ(task, "disable")){
        //Load project data
        fd::ProjectFile project(PROJECT_FILE);
        bool fast = project.type == pr::PROJECT_TYPE_SOLUTION;

        //Get [WHAT] and pop it
        if(argc <= 0)
            dtk::log::fatal_error("No arguments.", 83); //ELOAD
        
        char* what = argv[0];
        argv++;argc--;

        //Select [WHAT] and disable it (if hadn't been yet)
        if(STRCMP_EQ(what, "log")  || STRCMP_EQ(what, "silent")){
            if(project.enabled & pr::PROJECT_ENABLE_LOG == 0){
                dtk::log::warning("\"" + std::string(what) + "\" already disabled.");
                return 0;
            }

            project.enabled &= ~pr::PROJECT_ENABLE_LOG;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else if(STRCMP_EQ(what, "test") || STRCMP_EQ(what, "tests")){
            if(project.enabled & pr::PROJECT_ENABLE_TESTS == 0){
                dtk::log::warning("\"" + std::string(what) + "\" already disabled.");
                return 0;
            }

            project.enabled &= ~pr::PROJECT_ENABLE_TESTS;
            ft::update_run_script("run", project.build, fast, project.enabled);
        } else{
            dtk::log::fatal_error("Invalid functionality name\"" + std::string(what) + "\"", 130); //ENOEXEC
        }

        //Save changes
        project.update();
    } else if(STRCMP_EQ(task, "add")){
        //Get [WHAT] and pop it
        if(argc <= 0)
            dtk::log::fatal_error("Excepted argument after 'add'.", 130); //ENOEXEC
        
        char* what = argv[0];
        argv++;argc--;

        //Load project
        fd::ProjectFile project(PROJECT_FILE);

        #define IF_IMPORT(NAME, LONG_NAME, TYPE) if(STRCMP_EQ(what, NAME) || STRCMP_EQ(what, LONG_NAME)){ std::string value; std::string module_name; module_add_import_load_args(value, module_name, argc, argv); module_add_import(value, pr::PROJECT_ITEM_##TYPE, module_name, project); }

        //Switch what
        if(STRCMP_EQ(what, "template")){
            std::vector<std::string> names;
            std::vector<fd::ProjectTemplate> templates;

            //Load args
            for(int i = 0; i < argc; i++){
                if(STRCMP_EQ(argv[i], "-n") || STRCMP_EQ(argv[i], "--name")){
                    if(++i >= argc)
                        dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD

                    names.push_back(argv[i]);
                } else{ //Load templates
                    auto tmpls = fd::ProjectTemplate::load(argv[i]); //Load templates

                    for(auto& tmpl: tmpls)
                        templates.push_back(tmpl);
                }
            }

            //if any name given then filter templates list
            if(!names.empty()){
                std::vector<fd::ProjectTemplate> tmpls;

                for(auto& name: names){
                    bool found = false;
                    for(auto& t: templates){
                        if(t.name == name){
                            tmpls.push_back(t);
                            found = true;
                            break;
                        }
                    }

                    if(!found)
                        dtk::log::warning("Template \'" + name + "\' not found.");
                }

                templates = tmpls;
            }

            //Update templates' file
            ft::update_templates_file(TEMPLATES_FILE, templates);
        } else if(STRCMP_EQ(what, "module")){
            //Error if project is non-modular
            if(project.type != pr::PROJECT_TYPE_PROJECT)
                dtk::log::fatal_error("Project is non-modular.", 130); //ENOEXEC
            
            fd::ProjectModulesFile modules(MODULES_FILE);

            //Load data
            std::string name;
            std::string tmpl;

            bool name_set = false;
            bool tmpl_set = false;

            //Load args
            for(int i = 0; i < argc; i++){
                if(STRCMP_EQ(argv[i], "-t") || STRCMP_EQ(argv[i], "--template")){ //Set template, return error if already setted
                    if(++i >= argc)
                        dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD

                    if(tmpl_set)
                        dtk::log::fatal_error("Template already setted.", 83); //ELOAD

                    tmpl = argv[i];
                    tmpl_set = true;
                } else if(STRCMP_EQ(argv[i], "-n") || STRCMP_EQ(argv[i], "--name")){ //Set name
                    if(++i >= argc)
                        dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i -1]) + "'.", 83); //ELOAD
                        
                    if(name_set)
                        dtk::log::fatal_error("Name already setted.", 83); //ELOAD

                    name = argv[i];
                    name_set = true;
                } else{
                    dtk::log::fatal_error("Unknown argument \"" + std::string(argv[i]) + "\"", 158); //EMVSPARM
                }
            }

            //Validate data
            if(!name_set)
                dtk::log::fatal_error("Name has not been specified.", 83); //ELOAD
            
            if(!tmpl_set)
                dtk::log::fatal_error("Template has not been specified.", 83); //ELOAD
            
            for(auto& module: modules.modules){
                if(module.name == name)
                    dtk::log::fatal_error("Module named '" + name + "' already exists.", 83); //ELOAD
            }
            
            //Load templates
            auto templates = fd::ProjectTemplate::load(TEMPLATES_FILE);

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

            //Add module to project
            modules.modules.push_back(used_templ.get_module(name, project.build));
            modules.update();

            //Build module
            ft::build_template(used_templ.compile(name, project.build));
            ft::update_make_script(".scripts/make.sh", modules, project.build);
        } else IF_IMPORT("bin", "binary", BIN)
        else IF_IMPORT("lib", "library", LIB)
        else IF_IMPORT("inc", "include", INC)
        else IF_IMPORT("res", "resource", RES)
        else IF_IMPORT("bin_dir", "binary_directory", BIN_DIR)
        else IF_IMPORT("lib_dir", "library_directory", LIB_DIR)
        else IF_IMPORT("inc_dir", "include_directory", INC_DIR)
        else IF_IMPORT("res_dir", "resource_directory", RES_DIR)
        else{
            dtk::log::fatal_error("Invalid [WHAT] parameter \"" + std::string(what) + "\".", 130); //ENOEXEC
        }

        #undef IF_IMPORT
    } else if(STRCMP_EQ(task, "-h") || STRCMP_EQ(task, "help")){
        std::cout << help << std::endl;
    } else{
        dtk::log::fatal_error(std::string("Invalid [TASK] parameter \"") + task + "\"", 130); //ENOEXEC
    }
    return 0;
}

void module_add_import_load_args(std::string& value, std::string& module_name, int argc, char** argv){
    bool value_set = false;
    bool module_set = false;

    //Load args
    for(int i = 0; i < argc; i++){
        if(STRCMP_EQ(argv[i], "-m") || STRCMP_EQ(argv[i], "--module")){ //Set module name, return error if already setted or project is non-modular
            if(module_set)
                dtk::log::fatal_error("Module already setted.", 83); //ELOAD

            if(++i >= argc)
                dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i - 1]) + "'.", 83); //ELOAD

            module_name = argv[i];
            module_set = true;
        } else if(STRCMP_EQ(argv[i], "-v") || STRCMP_EQ(argv[i], "--value")){ //Set path
            if(value_set)
                dtk::log::fatal_error("Value already setted.", 83); //ELOAD

            if(++i >= argc)
                dtk::log::fatal_error("Excepted argument after '" + std::string(argv[i -1]) + "'.", 83); //ELOAD

            value = argv[i];
            value_set = true;
        } else{
            dtk::log::fatal_error("Unknown argument \"" + std::string(argv[i]) + "\"", 158); //EMVSPARM
        }
    }

    //Validate data
    if(!value_set)
        dtk::log::fatal_error("Value has not been setted.", 83); //ELOAD
}

void module_add_import(const std::string& val, pr::item_type t, const std::string module_name, const fd::ProjectFile& project){
    //Load templates
    auto templates = fd::ProjectTemplate::load(TEMPLATES_FILE);

    if(project.type == project::PROJECT_TYPE_PROJECT){
        //Load modules
        fd::ProjectModulesFile modules(MODULES_FILE);

        //Add lib to selected or all modules
        if(!module_name.empty()){
            bool found = false;

            for(auto& m: modules.modules){
                if(m.name == module_name){
                    found = true;

                    //Find template
                    bool tmpl_err = true;

                    for(auto& t: templates){
                        if(t.name == m.tmpl_name){
                            tmpl_err = false;

                            m.add_import(val, project::PROJECT_ITEM_LIB, project.build, t);

                            break;
                        }
                    }

                    if(tmpl_err)
                        dtk::log::fatal_error("Template '" + m.tmpl_name + "' used by module '" + m.name + "' not found.", 83); //ELOAD

                    break;
                }
            }

            if(!found)
                dtk::log::fatal_error("Module not found '" + module_name + "'.", 83); //ELOAD
        } else{
            for(auto& m: modules.modules){
                //Find template
                bool tmpl_err = true;

                for(auto& t: templates){
                    if(t.name == m.tmpl_name){
                        tmpl_err = false;

                        m.add_import(val, project::PROJECT_ITEM_LIB, project.build, t);

                        break;
                    }
                }

                if(tmpl_err)
                    dtk::log::fatal_error("Template '" + m.tmpl_name + "' used by module '" + m.name + "' not found.", 83); //ELOAD
            }
        }

        //Update changes
        modules.update();
    } else{
        if(!module_name.empty())
            dtk::log::fatal_error("Project is non-modular.", 83); //ELOAD

        dtk::log::fatal_error("Not supported yet...", -1); //???
    }
}