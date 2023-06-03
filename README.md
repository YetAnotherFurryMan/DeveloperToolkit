# DeveloperToolkit
A set of tools that (I wish to) help me in my programmer's poor life. Project creation, logging, you get the point.

# 1.common
A set of common functions and classes/structures.
<ol>
  <li>dtk::common::str::replase(std::string& s, const std::string& from, const std::string& to) <br> replases first <i>from</i> to <i>to</i> in <i>s</i></li>
  <li>dtk::common::str::replase_all(std::string& s, const std::string& from, const std::string& to) <br> replases all <i>from</i> to <i>to</i> in <i>s</i></li>
  <li>dtk::common::str::clean_beginning(std::string& s) <br> removes spacies and tabs from the begining of <i>s</i></li>
  <li>dtk::common::str::CString <br> a struct for allocating and freeing char buffer (C like string)</li>
</ol>

# 2.log
A logging function set. Still work-in-progress.
<ol>
  <li>dtk::log::terminate(int code) <br> prints "Terminated." and exits with given <i>code</i></li>
  <li>dtk::log::error(const std::string& msg, int code = 0) <br> dtk::log::error(const char* msg, int code = 0) <br> prints "Error: " then <i>msg</i> and if <i>code</i> != 0 then <i>terminate(code)</i></li>
  <li>dtk::log::fatal_error(const std::string& msg, int code = 0) <br> dtk::log::fatal_error(const char* msg, int code = 0) <br> the same as error but prints "Fatal error: "</li>
  <li>dtk::log::warning(const std::string& msg) <br> dtk::log::warning(const char* msg) <br> prints "Warning: " then <i>msg</i></li>
  <li>dtk::log::info(const std::string& msg) <br> dtk::log::info(const char* msg) <br> prints "Info: " then <i>msg</i></li>
  <li>dtk::log::assert(bool b, const std::string& msg, int code = 0) <br> dtk::log::assert(bool b, const char* msg, int code = 0) <br> inline functions; only if DTK_LOG_ENABLE_ASSERTION defined; if <i>b</i> then prints "Assertion: " then <i>msg</i> and <i>terminate(code)</i> if <i>code</i> != 0</li>
  <li>dtk::log::assert_errno() <br> inline function; the same as assert but the <i>code</i> is <i>errno</i> and the <i>msg</i> is <i>strerror(errno)</i> only if <i>errno</i> != 0</li>
</ol>

# 3.ml
Well, XML - bad, JSON - bad, so I wrote a lexer+tookenizer+parser for a new markup language!</br>
This is used by the <b>project</b> module for template-files, module-files and config.
<ol>
  <il>dtk::ml::MLAttribute <br> a struct; holds data abound attribute/definition/modifier (simply <i>name</i> and <i>value</i> or 0)</li>
  <il>dtk::ml::MLSection <br> a struct; holds data abound section (<i>name</i>, <i>definitions</i>, <i>attributes</i>, <i>modifiers</i>, sub<i>sections</i> and <i>values</i>)</li>
  <il>dtk::ml::MLRoot <br> a struct; holds first data in file (<i>definitions</i>, <i>attributes</i> and <i>sections</i>)</li>
  <il>dtk::ml::ml_load_file(FILE* f) <br> returns a pointer to MLRoot with data loaded from file <i>f</i></li>
  <il>dtk::ml::ml_root_to_str(struct MLRoot* r) <br> returns char* to C like string with "decompiled" MLRoot <i>r</i></li>
  <il>dtk::ml::ml_free_root(struct MLRoot* r) <br> frees memory</li>
</ol>

# 4.project
One of the "core" tools of this project. </br>
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
  <li>template - adds a template file to the '.project/templates' file.</li>
  <li>module - creates a new module (following the choosen template-file) and adds it to the project.</li>
</ol>
