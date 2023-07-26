# DONE:
<ol>
    <li>Fixed test.sh and replased project -> mpm and pr_% -> mpm_% and _pr -> _mpm</li>
    <li>Added assertion macros to log and log++ when DTK_LOG_ENABLE_ASSERTION is not defined</li>
    <li>Removed dtkmlc</li>
    <li>Added find functions to ml</li>
    <li>Removed MLRoot struct from ml</li>
    <li>Added MLAttributeBuffer, MLValueBuffer and MLSectionBuffer structures to ml (insted of NAME_no and NAMEs varibles)</li>
    <li>Added module testing to test.sh</li>
    <li>Added title support to mpm templates</li>
    <li>Reformated test script</li>
    <li>Fixed doubled '\n' in ml's string builder</li>
    <li>Added ml_create_inheritance function to ml</li>
    <li>Added ml_debuplicate_attributes function to ml</li>
    <li>Added #undef and inline functions insted of #define to ml.hpp</li>
    <li>Added ml_destroy_inheritance function to ml</li>
    <li>Added struct MLPointerBuffer and converters to and from it to ml</li>
    <li>Added ml_optimize, ml_remove and ml_add function to ml</li>
    <li>Added ml_optimize% like functions</li>
    <li>Rewritted functions like ml_add% and ml_remove% (they are converting given buffer to MLPointerBuffer and calling to corresponding func)</li>
    <li>Added a full list of functions, macros and structs in ml to readme</li>
</ol>

# TODO:
## Version 1.0
<ol>
    <li>Add tests support to mpm</li>
    <li>Add additional support to mpm</li>
    <li>Add support for cmake to mpm</li>
    <li>Add support for git to mpm</li>
</ol>

## Version 1.1
<ol>
    <li>Move this project to mpm</li>
    <li>Add github support to mpm</li>
    <li>Add templates for:
        <ul>
            <li>Flex(C, C++)</li>
            <li>Bison(C, C++)</li>
            <li>C(lib, exe, resgen)</li>
            <li>C++(lib, exe, resgen)</li>
            <li>Rust(lib, rlib, exe, resgen)</li>
            <li>Java(lib, jar, resgen)</li>
            <li>Fortran(lib, exe, resgen)</li>
            <li>Lua(lib, exe, resgen, res)</li>
            <li>OCaml(lib, exe, resgen)</li>
            <li>Python(exe, resgen)</li>
        </ul>
    </li>
</ol>

## Version 2.0
<ol>
    <li>Add simple White-Box test macros to log and log++</li>
    <li>Add doc module</li>
</ol>