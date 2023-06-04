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
</ol>

# Todo:
<ol>
    <li>Add support of xml files generation to dtklc</li>
    <li>project using ml</li>
    <li>project keep track of used libs</li>
    <li>Add project export</li>
</ol>
