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
Well, XML - bad, JSON - bad, TOML - bad, so I wrote a new markup language!</br>
This is used by the <b>mpm</b> module for template-files, module-files and config.
<ol>
  <li>dtk::ml::MLAttribute <br> a struct; holds data about attribute/definition/modifier (simply <i>name</i> and <i>value</i> or 0)</li>
  <li>dtk::ml::MLSection <br> a struct; holds data about section (<i>name</i>, <i>definitions</i>, <i>attributes</i>, <i>modifiers</i>, sub<i>sections</i> and <i>values</i>)</li>
  <li>dtk::ml::MLValueBuffer <br> a struct; holds array of char*, the number of items and the size of a buffer</li>
  <li>dtk::ml::MLAttributeBuffer <br> a struct; holds array of MLAttribute*, the number of items and the size of a buffer</li>
  <li>dtk::ml::MLSectionBuffer <br> a struct; holds array of MLSection*, the number of items and the size of a buffer</li>
  <li>dtk::ml::ml_load_file(FILE* f) <br> returns a pointer to MLSection with data loaded from file <i>f</i> and name "root"</li>
  <li>dtk::ml::ml_root_to_str(struct MLSection* r) <br> returns char* to C like string with stringify MLSection <i>r</i></li>
  <li>dtk::ml::ml_free_root(struct MLSection* r) <br> frees memory</li>
  <li>dtk::ml::ml_find(const MLAttributeBuffer* b, const char* name) <br> returns an index of attribute witch name is equal to <i>name</i> or -1</li>
  <li>dtk::ml::ml_find_section(const MLSectionBuffer* b, const char* name) <br> returns an index of section witch name is equal to <i>name</i> or -1</li>
  <li>dtk::ml::ml_find_value(const MLValueBuffer* b, const char* name) <br> returns an index of value what is equal to <i>name</i> or -1</li>
  <li>dtk::ml::ml_find_attribute(const MLSection* s, const char* name) <br> a macro or inline function; calls ml_find with <i>s->attributes</i> and <i>name</i></li>
  <li>dtk::ml::ml_find_modifier(const MLSection* s, const char* name) <br> a macro or inline function; calls ml_find with <i>s->modifiers</i> and <i>name</i></li>
  <li>dtk::ml::ml_find_definition(const MLSection* s, const char* name) <br> a macro or inline function; calls ml_find with <i>s->definitions</i> and <i>name</i></li>
  <li>
    With DTK_ML_INHERITANCE defined:
    <ul>
      <li>dtk::ml::ml_create_inheritance(MLSection* s) <br> creates definitions' inheritance for <i>s</i></li>
      <li>dtk::ml::ml_destroy_inheritance(MLSection* s, MLAttributeBuffer* b) <br> destroys definitions' inheritance for <i>s</i></li>
      <li>dtk::ml::ml_deduplicate_attributes(MLAttributeBuffer* b) <br> removes duplicated MLAttribute (by name) from <i>b</i></li>
    </ul>
  </li>
  <li>
    With DTK_ML_STR_BUILDER defined:
    <ul>
      <li>dtk::ml::ml_put(MLAttribute* d, char* str, const char* prefix, const char initializer, const char* equalizer, const char* terminator) <br> stringify an attribute <i>d</i></li>
      <li>dtk::ml::ml_put_section(MLSection* s, char* str, const char* prefix) <br> stringify a section</li>
      <li>dtk::ml::ml_put_value(const char* v, char* str, const char* prefix) <br> stringify a value</li>
      <li>dtk::ml::ml_put_definition(MLAttribute* d, char* str, const char* prefix) <br> a macro/inline function; calls ml_put</li>
      <li>dtk::ml::ml_put_attribute(MLAttribute* a, char* str, const char* prefix) <br> a macro/inline function; calls ml_put</li>
      <li>dtk::ml::ml_put_modifier(MLAttribute* m, char* str) <br> a macro/inline function; calls ml_put</li>
    </ul>
  </li>
  <li>
    With DTK_ML_BUFFERS defined:
    <ul>
      <li>dtk::ml::MLPointerBuffer <br> a struct; holds elements' number, array's size, element's size (type) and array</li>
      <li>dtk::ml::ml_convert_to_attribute_buffer(MLPointerBuffer* b) <br> converts <i>b</i> to MLAttributeBuffer</li>
      <li>dtk::ml::ml_convert_to_section_buffer(MLPointerBuffer* b) <br> converts <i>b</i> to MLSectionBuffer</li>
      <li>dtk::ml::ml_convert_to_value_buffer(MLPointerBuffer* b) <br> converts <i>b</i> to MLValueBuffer</li>
      <li>dtk::ml::ml_add(MLPointerBuffer* b, void* element) <br> adds <i>element</i> to <i>b</i></li>
      <li>dtk::ml::ml_remove(MLPointerBuffer* b, size_t index) <br> removes element at <i>index</i> from <i>b</i></li>
      <li>dtk::ml::ml_optimize(MLPointerBuffer* b) <br> reallocates <i>b</i>'s array if its size is bigger than minimal needed size</li>
      <li>dtk::ml::ml_convert_from_attribute_buffer_ptr(MLAttributeBuffer* b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
      <li>dtk::ml::ml_convert_from_section_buffer_ptr(MLSectionBuffer* b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
      <li>dtk::ml::ml_convert_from_value_buffer_ptr(MLValueBuffer* b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
      <li>dtk::ml::ml_convert_from_attribute_buffer(MLAttributeBuffer& b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
      <li>dtk::ml::ml_convert_from_section_buffer(MLSectionBuffer& b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
      <li>dtk::ml::ml_convert_from_value_buffer(MLValueBuffer& b) <br> a macro/inline function; converts <i>b</i> to MLPointerBuffer</li>
    </ul>
  </li>
  <li>
    With DTK_ML_DTK_BUILDER defined:
    <ul>
      <li>dtk::ml::ml_new_attribute(const char* name, const char* value) <br> allocates a new MLAttribute with <i>name</i> and <i>value</i></li>
      <li>dtk::ml::ml_new_section() <br> allocates a new MLSection</li>
      <li>dtk::ml::ml_add_attribute(MLAttributeBuffer* b, MLAttribute* a) <br> adds <i>a</i> to <i>b</i></li>
      <li>dtk::ml::ml_add_section(MLSectionBuffer* b, MLSection* s) <br> adds <i>s</i> to <i>b</i></li>
      <li>dtk::ml::ml_add_value(MLValueBuffer* b, char* v) <br> adds <i>v</i> to <i>b</i></li>
      <li>dtk::ml::ml_remove_attribute(MLAttributeBuffer* b, size_t index) <br> removes the element at <i>index</i> from <i>b</i> (calls ml_remove)</li>
      <li>dtk::ml::ml_remove_section(MLSectionBuffer* b, size_t index) <br> removes the element at <i>index</i> from <i>b</i> (calls ml_remove)</li>
      <li>dtk::ml::ml_remove_value(MLValueBuffer* b, size_t index) <br> removes the element at <i>index</i> from <i>b</i> (calls ml_remove)</li>
      <li>dtk::ml::ml_optimize_attributes(MLAttributeBuffer* b) <br> reallocates <i>b</i>'s array if its size is bigger than minimal needed size (calls ml_optimize)</li>
      <li>dtk::ml::ml_optimize_sections(MLSectionBuffer* b) <br> reallocates <i>b</i>'s array if its size is bigger than minimal needed size (calls ml_optimize)</li>
      <li>dtk::ml::ml_optimize_values(MLValueBuffer* b) <br> reallocates <i>b</i>'s array if its size is bigger than minimal needed size (calls ml_optimize)</li>
      <li>dtk::ml::ml_copy_attribute(MLAttribute* a) <br> a macro/inline function; calls ml_new_attribute with <i>a->name</i> and <i>a->value</i></li>
    </ul>
  </li>
  <li>
    With DTK_ML_ELEMENT_FREE defined:
    <ul>
      <li>dtk::ml::ml_free_attribute(MLAttribute* a) <br> frees <i>a</i> and its name and value</li>
      <li>dtk::ml::ml_free_section(MLSection* s) <br> frees <i>s</i> and its elements</li>
      <li>dtk::ml::ml_free_value(char* v) <br> a macro/inline function; calls free with <i>v</i></li>
    </ul>
  </li>
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
