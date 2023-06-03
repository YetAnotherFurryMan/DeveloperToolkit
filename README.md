# DeveloperToolkit
A set of tools that (I wish to) help me in my programmer's poor life. Project creation, logging, you get the point.

# 1.common
A set of common functions and classes/structures for the project.

# 2.log
A logging function set. Still work-in-progress.

# 3.ml
Well, XML - bad, JSON - bad, so I wrote a lexer+tookenizer+parser for a new markup language!</br>
This is used by the <b>project</b> module for template-files, module-files and config.

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
