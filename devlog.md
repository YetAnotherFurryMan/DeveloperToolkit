## Version Betha 3.0
# Done:
<ol>
    <li>Renamed DataStructure => ml (Markup Language)</li>
    <li>Added ml_format_str to common</li>
    <li>Fixed ml value writting (to_str.c)</li>
    <li>Added dtkmlc module (DTK Markup Language Compiler)</li>
    <li>Added support of txt files generation to dtkmlc</li>
    <li>Moved ml's test/test target make from make=>all to test.sh</li>
    <li>Fixed README.md</li>
    <li>Seperated common (C version) and common++ (C++ version)</li>
    <li>Extended dtk::common::str::CString</li>
    <li>Renamed log => log++</li>
    <li>Added log (C version of log++)</li>
    <li>Deleted all the functions witch const char* argument from log++</li>
    <li>Moved ml_format_str from common to ml</li>
    <li>Removed 'exit' from .scripts/test.sh</li>
    <li>Updated .gitignore</li>
    <li>project is using ml to interpret template files</li>
    <li>Added dtk_str_copy(char*) to common</li>
    <li>Added XML support to dtkmlc</li>
    <li>ml_fromat_str(char* s) uses \xXX notation for any character c smaller than 32 and bigger than 128</li>
    <li>ml lexer know \t and \r</li>
    <li>Both LONG_VALUE and VALUE are now formated in lexer</li>
    <li>Added .scripts/clean.sh</li>
    <li>Setted script's execution method to bash {path}</li>
    <li>Defined PROJECT_FILE, TEMPLATES_FILE, MODULES_FILE in defs.hpp in the project module with default project, templates, modules files paths</li>
    <li>In the project module function ProjectTemplate::load calls dtk::log::warning (insted of dtk::log::error) in "failed to open template file" case</li>
    <li>The test script doesn't show NON_MODULAR projects' (the test objects') output now. Insted prints "Test $NAME - " and "passed" or "failed"</li>
    <li>The project's modules file uses the ml to keep data about modules in projects</li>
    <li>Added 'help' task to the project module</li>
    <li>Reimplemented the templates' adding process and moved to seperated task (add) with a [WHAT] parameter (template)</li>
    <li>Reimplemented the modules' adding process and moved to seperated task (add) with a [WHAT] parameter (module)</li>
    <li>Updated 'project info' command. Lists modules and templates. Prints warnings if 'no templates found' and 'no modules found'. Exits before listing templates and modules if the project is non-modular</li>
    <li>Deleted link tag support for template files</li>
    <li>Added type and priority modifiers support to template files</li>
    <li>Added summary to test script</li>
    <li>When a new module is in creation process, if the name was found in already existing modules then project trows a fatal error</li>
    <li>Added 'project add library'</li>
    <li>Added 'project add [WHAT]' for WHAT in ['binary', 'library', 'include', 'resource', 'binary_directory', 'library_directory', 'include_directory', 'resource_directory']</li>
    <li>Added new template 'c_math'</li>
</ol>

# Todo:
<ol>
    <li>DONE!</li>
</ol>

# Moved to next version
<ol>
    <li>In ml subsections extends its supersection's definitions</li>
    <li>Add git support to project</li>
    <li>Add "config" section support to project</li>
    <li>Add name modifier to rule and link in template files</li>
    <li>Add "additional" section support to project</li>
</ol>