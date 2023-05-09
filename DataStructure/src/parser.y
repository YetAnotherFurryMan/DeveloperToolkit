%{
    #include "../headers/DTKDS.h"

    #include <malloc.h>
    #include <string.h>

    struct DSRoot root_node = {0,0,0,0,0,0,0,0};

    struct DSRoot* get_root();
%}

%define api.value.type union

%token <char*> TK_ID
%token <char*> TK_VALUE

%type <struct DSSection*> section;
%type <struct { size_t no; struct DSAttribute** mods; }> section_modifiers;
%type <struct DSSection*> section_in;

%type <struct DSAttribute*> attribute;
%type <struct DSAttribute*> modifier;

%type <struct DSAttribute*> definition;

%%

root:
      section root {
        root_node.section_no++;

        if(root_node.sections == 0)
            root_node.sections = malloc(sizeof(struct DSSection*));
        else
            root_node.sections = realloc(root_node.sections, (sizeof(struct DSSection*)) * root_node.section_no);
        
        root_node.sections[root_node.section_no - 1] = $1;
      }
    | '!' attribute root {
        root_node.attribute_no++;

        if(root_node.attributes == 0)
            root_node.attributes = malloc(sizeof(struct DSAttribute*));
        else 
            root_node.attributes = realloc(root_node.attributes, (sizeof(struct DSAttribute*)) * root_node.attribute_no);
        
        root_node.attributes[root_node.attribute_no - 1] = $2;
      }
    | '#' definition root {
        root_node.definition_no++;

        if(root_node.definitions == 0)
            root_node.definitions = malloc(sizeof(struct DSAttribute*));
        else 
            root_node.definitions = realloc(root_node.definitions, (sizeof(struct DSAttribute*)) * root_node.definition_no);
        
        root_node.definitions[root_node.definition_no - 1] = $2;
      }
    | '#' attribute root {
        root_node.varible_no++;

        if(root_node.varibles == 0)
            root_node.varibles = malloc(sizeof(struct DSAttribute*));
        else 
            root_node.varibles = realloc(root_node.varibles, (sizeof(struct DSAttribute*)) * root_node.varible_no);
        
        root_node.varibles[root_node.varible_no - 1] = $2;
      }
    | %empty
;

section:
    '<' TK_ID section_modifiers '>' section_in '<' '/' TK_ID '>' {
        if(strcmp($2, $8) != 0){
            printf("Syntax error: Excepted </%s> (Found </%s>)\n", $2, $8);
            return -1;
        }

        $$ = $5;

        $$->name = malloc(strlen($2));
        strcpy($$->name, $2);

        if($$->modifiers == 0){
            $$->modifier_no = $3.no;
            $$->modifiers = $3.mods;
        } else{
            struct DSAttribute** temp = malloc(sizeof(struct DSAttribute*) * ($$->modifier_no + $3.no));

            memcpy(temp, $3.mods, sizeof(struct DSAttribute*) * $3.no);
            memcpy(&(temp[$3.no]), $$->modifiers, sizeof(struct DSAttribute*) * $$->modifier_no);
            
            free($3.mods);
            free($$->modifiers);

            $$->modifier_no += $3.no;
            $$->modifiers = temp;
        }
    }
;

section_modifiers:
      section_modifiers modifier {
        $$ = $1;
        $$.no++;

        if($$.mods == 0)
            $$.mods = malloc(sizeof(struct DSAttribute*));
        else
            $$.mods = realloc($$.mods, sizeof(struct DSAttribute*) * $$.no);

        $$.mods[$$.no - 1] = $2;
      }
    | %empty { $$.no = 0; $$.mods = 0; }
;

section_in:
      section_in section {
        $$ = $1;

        $$->section_no++;

        if($$->sections == 0)
            $$->sections = malloc(sizeof(struct DSSection*));
        else
            $$->sections = realloc($$->sections, sizeof(struct DSSection*) * $$->section_no);
        
        $$->sections[$$->section_no - 1] = $2;
      }
    | section_in '!' attribute {
        $$ = $1;

        $$->attribute_no++;

        if($$->attributes == 0)
            $$->attributes = malloc(sizeof(struct DSAttribute*));
        else
            $$->attributes = realloc($$->attributes, sizeof(struct DSAttribute*) * $$->attribute_no);
        
        $$->attributes[$$->attribute_no - 1] = $3;
      }
    | section_in '!' modifier {
        $$ = $1;

        $$->modifier_no++;

        if($$->modifiers == 0)
            $$->modifiers = malloc(sizeof(struct DSAttribute*));
        else
            $$->modifiers = realloc($$->modifiers, sizeof(struct DSAttribute*) * $$->modifier_no);
        
        $$->modifiers[$$->modifier_no - 1] = $3;
      }
    | section_in TK_VALUE {
        $$ = $1;

        $$->value_no++;

        if($$->values == 0)
            $$->values = malloc(sizeof(char*));
        else
            $$->values = realloc($$->values, sizeof(char*) * $$->value_no);
        
        $$->values[$$->value_no - 1] = $2;
      }
    | %empty { $$ = malloc(sizeof(struct DSSection)); }
;

attribute:
    TK_ID ':' TK_VALUE {
        $$ = malloc(sizeof(struct DSAttribute));

        $$->name = malloc(strlen($1));
        strcpy($$->name, $1);

        $$->value = malloc(strlen($3));
        strcpy($$->value, $3);
    }
;

definition:
      modifier { $$ = $1; }
    | TK_ID {
        $$ = malloc(sizeof(struct DSAttribute));

        $$->name = malloc(strlen($1));
        strcpy($$->name, $1);

        $$->value = 0;
    }
;

modifier:
    TK_ID '=' TK_VALUE {
        $$ = malloc(sizeof(struct DSAttribute));

        $$->name = malloc(strlen($1));
        strcpy($$->name, $1);

        $$->value = malloc(strlen($3));
        strcpy($$->value, $3);
    }
;

%%
/*
root:
      section root {}
    | attribute root {}
    | global root {}
    | definition root {}
    | %empty
;

section:
    '<' TK_ID '>' section_in '<' '/' TK_ID '>' {
        printf("Parser: <%s>...</%s>\n", $2, $7);
    }
;

section_in:
    section section_in {}
    | attribute section_in {}
    | modifier section_in {}
    | %empty { $$ = malloc(sizeof DSSection); }
;

%%*/

struct DSRoot* get_root(){
    return &root_node;
}