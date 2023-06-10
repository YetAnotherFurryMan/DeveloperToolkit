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
    <li>Setted execution method to bash {path}</li>
</ol>

# Todo:
<ol>
    <li>project keep track of used libs</li>
</ol>

# Moved to next version
<ol>
    <li>Add module exporting to project</li>
    <li>Add git support to project</li>
    <li>Add release export to project</li>
    <li>Add debug export to project</li>
    <li>Add name modifier to rule and link in template files</li>
    <li>Add additional section to template file</li>
    <li>Reorganize template files' structure</li>
</ol>