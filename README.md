# DeveloperToolkit
A set of tools that (I wish to) help me in my programmer's poor life. Project creation, logging, you get the point.

## 1.common
A set of common functions and structures for C.
<ol>
  <li>common_str_copy(char* s) <br> returns a new-allocated char buffer with data from <i>s</i>, in other words: returns a copy of <i>s</i></li>
</ol>

## 2.common
A set of common functions and classes/structures for C++.
<ol>
  <li>dtk::common::str::replase(std::string& s, const std::string& from, const std::string& to) <br> replases first <i>from</i> to <i>to</i> in <i>s</i></li>
  <li>dtk::common::str::replase_all(std::string& s, const std::string& from, const std::string& to) <br> replases all <i>from</i> to <i>to</i> in <i>s</i></li>
  <li>dtk::common::str::clean_beginning(std::string& s) <br> removes spacies and tabs from the begining of <i>s</i></li>
  <li>dtk::common::str::CString <br> a struct for char buffer menagment (C like string)</li>
</ol>

## 3.log
A set of logging functions for C. Still work-in-progress
<ol>
  <li>log_terminate(int code) <br> prints "Terminated." and exits with given <i>code</i></li>
  <li>log_error(const char* msg, int code) <br> prints "Error: " then <i>msg</i> and if <i>code</i> != 0 then <i>terminate(code)</i></li>
  <li>log_fatal_error(const char* msg, int code) <br> the same as error but prints "Fatal error: "</li>
  <li>log_warning(const char* msg) <br> prints "Warning: " then <i>msg</i></li>
  <li>log_info(const char* msg) <br> prints "Info: " then <i>msg</i></li>
  <li>log_assert(char b, const char* msg, int code) <br> only if DTK_LOG_ENABLE_ASSERTION defined; if <i>b</i> then prints "Assertion: " then <i>msg</i> and <i>terminate(code)</i> if <i>code</i> != 0</li>
  <li>log_assert_errno() <br> calls assert where <i>b</i> = <i>errno</i> != 0, <i>code</i> = <i>errno</i> and <i>msg</i> = <i>strerror(errno)</i></li>
</ol>

## 4.log++
A set of logging functions for C++. Still work-in-progress.
<ol>
  <li>dtk::log::terminate(int code) <br> prints "Terminated." and exits with given <i>code</i></li>
  <li>dtk::log::error(const std::string& msg, int code = 0) <br> prints "Error: " then <i>msg</i> and if <i>code</i> != 0 then <i>terminate(code)</i></li>
  <li>dtk::log::fatal_error(const std::string& msg, int code = 0) <br> the same as error but prints "Fatal error: "</li>
  <li>dtk::log::warning(const std::string& msg) <br> prints "Warning: " then <i>msg</i></li>
  <li>dtk::log::info(const std::string& msg) <br> prints "Info: " then <i>msg</i></li>
  <li>dtk::log::assert(bool b, const std::string& msg, int code = 0) <br> only if DTK_LOG_ENABLE_ASSERTION defined; if <i>b</i> then prints "Assertion: " then <i>msg</i> and <i>terminate(code)</i> if <i>code</i> != 0</li>
  <li>dtk::log::assert_errno() <br> inline function; calls assert where <i>b</i> = <i>errno</i> != 0 <i>code</i> = <i>errno</i> and <i>msg</i> = <i>strerror(errno)</i></li>
</ol>

## 5.ml
Well, XML - bad, JSON - bad, TOML - bad, so I wrote a lexer+tookenizer+parser for a new markup language!</br>
This is used by the <b>mpm</b> module for template-files, module-files and config.
<ol>
  <li>dtk::ml::MLAttribute <br> a struct; holds data abound attribute/definition/modifier (simply <i>name</i> and <i>value</i> or 0)</li>
  <li>dtk::ml::MLSection <br> a struct; holds data abound section (<i>name</i>, <i>definitions</i>, <i>attributes</i>, <i>modifiers</i>, sub<i>sections</i> and <i>values</i>)</li>
  <li>dtk::ml::MLRoot <br> a struct; holds first data in file (<i>definitions</i>, <i>attributes</i> and <i>sections</i>)</li>
  <li>dtk::ml::ml_load_file(FILE* f) <br> returns a pointer to MLRoot with data loaded from file <i>f</i></li>
  <li>dtk::ml::ml_root_to_str(struct MLRoot* r) <br> returns char* to C like string with "decompiled" MLRoot <i>r</i></li>
  <li>dtk::ml::ml_free_root(struct MLRoot* r) <br> frees memory</li>
</ol>

## 6.mpm
Modular Projects' Manager - one of the "core" tools of this project. </br>
Tasks:
<ol>
  <li>init - initializes a new project directory-tree.</li>
  <li>info - prints information on the project.</li>
  <li>enable - enables a feature.
    <ul>
      <li>log/silent - all log info goes to a file.</li>
      <li>test/tests - adds a '.sctipts/test.sh' execution to 'run' file.</li>
    </ul>
  </li>
  <li>disable - oposite for enable.</li>
  <li>add - adds [WHAT] to the project or to the selected module.
    <ul>
      <li>template - adds a template file to the '.project/templates' file.</li>
      <li>module - adds a new module (following the choosen template-file).</li>
      <li>binary - adds a selected binary file to the binarys' list in the project's modules or one selected module.</li>
      <li>library - adds a selected library file (or simply name in C/C++ case) to the librarys' list in the project's modules or one selected module.</li>
      <li>include - adds a selected header file to the includes' list in the project's modules or one selected module.</li>
      <li>resource - adds a selected resource file to the resources' list in the project's modules or one selected module.</li>
      <li>binary_directory - adds a selected directory file to the binarys' directories list in the project's modules or one selected module.</li>
      <li>library_directory - adds a selected directory file (or simply name in C/C++ case) to the librarys' directories list in the project's modules or one selected module.</li>
      <li>include_directory - adds a selected directory file to the includes' directories list in the project's modules or one selected module.</li>
      <li>resource_directory - adds a selected directory file to the resources' directories list in the project's modules or one selected module.</li>
    </ul>
  </li>
  <li>help - print hepl page and exits.</li>
</ol>
