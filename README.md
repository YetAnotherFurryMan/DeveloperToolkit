# DeveloperToolkit
A set of tools that (I wish to) help me in my programmer poor life. Project creation, logging, you get the point.

# 1.common
A set of common functions and classes/structures for project.

# 2.log
Some logging function set. Still work-in-progress.

# 3.DataStructure
Well, XML - bad, JSON - bad, so I had wrote a lexer+tookenizer+parser for a new data structure!</br>
This is used by <b>project</b> module for template-files reading and writting.

# 4.project
One of the "core" tools of this projest. </br>
Tasks:
<ol>
  <li>init - initializes a new project directory-tree.</li>
  <li>info - prints informations abound project.</li>
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
