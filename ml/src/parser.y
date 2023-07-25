%{
    #define DTK_ML_DTK_BUILDER
    #include "../headers/ml.h"

    #include <malloc.h>
    #include <string.h>

    MLSection* root_node = 0;
    MLSection* ml_get_root();
%}

%define api.prefix {ml_yy}

%define api.value.type union

%token <char*> TK_ID
%token <char*> TK_VALUE

%type <MLSection*> section;
%type <struct { size_t no; MLAttribute** mods; }> section_modifiers;
%type <MLSection*> section_in;

%type <MLAttribute*> attribute_value;

%type <MLAttribute*> definition;
%type <MLAttribute*> attribute;
%type <MLAttribute*> modifier;

%%

root:
      root section {
        if(!root_node){
            root_node = ml_new_section();
            root_node->name = "dtk";
        }

        ml_add_section(&root_node->sections, $2);
      }
    | root attribute {
        if(!root_node){
            root_node = ml_new_section();
            root_node->name = "dtk";
        }
        
        ml_add_attribute(&root_node->attributes, $2);
      }
    | root definition {
        if(!root_node){
            root_node = ml_new_section();
            root_node->name = "dtk";
        }
        
        ml_add_attribute(&root_node->definitions, $2);
      }
    | %empty
;

section:
    '<' TK_ID section_modifiers '>' section_in '<' '/' TK_ID '>' {
        if(strcmp($2, $8) != 0){
            fprintf(stderr, "Syntax error: Excepted </%s> (Found </%s>)\n", $2, $8);
            return -1;
        }

        $$ = $5;

        $$->name = malloc(strlen($2));
        strcpy($$->name, $2);

        if($$->modifiers.array == 0){
            $$->modifiers.no = $3.no;
            $$->modifiers.array = $3.mods;
        } else{
            MLAttribute** temp = malloc(sizeof( MLAttribute*) * ($$->modifiers.no + $3.no));

            memcpy(temp, $3.mods, sizeof(MLAttribute*) * $3.no);
            memcpy(&(temp[$3.no]), $$->modifiers.array, sizeof(MLAttribute*) * $$->modifiers.no);
            
            free($3.mods);
            free($$->modifiers.array);

            $$->modifiers.no += $3.no;
            $$->modifiers.array = temp;
        }
    }
;

section_modifiers:
      section_modifiers modifier {
        $$ = $1;
        $$.no++;

        if($$.mods == 0)
            $$.mods = malloc(sizeof(MLAttribute*));
        else
            $$.mods = realloc($$.mods, sizeof(MLAttribute*) * $$.no);

        $$.mods[$$.no - 1] = $2;
      }
    | %empty { $$.no = 0; $$.mods = 0; }
;

section_in:
      section_in section {
        $$ = $1;
        ml_add_section(&$$->sections, $2);
      }
    | section_in attribute {
        $$ = $1;
        ml_add_attribute(&$$->attributes, $2);
      }
    | section_in definition {
        $$ = $1;
        ml_add_attribute(&$$->definitions, $2);
      }
    | section_in TK_VALUE {
        $$ = $1;
        ml_add_value(&$$->values, $2);
      }
    | %empty { 
        $$ = ml_new_section();
    }
;

is: '=' | ':';

attribute_value:
      TK_ID is TK_VALUE {
        $$ = ml_new_attribute($1, $3);
      }
    | TK_ID {
        $$ = ml_new_attribute($1, 0);
    }
;

attribute:
    '!' attribute_value {
        $$ = $2;
    }
;

definition:
    '#' attribute_value {
        $$ = $2;
    }
;

modifier:
      TK_ID '=' TK_VALUE {
        $$ = ml_new_attribute($1, $3);
      }
    | TK_ID {
        $$ = ml_new_attribute($1, 0);
    }
;

%%

MLSection* ml_get_root(){
    MLSection* root = root_node;
    root_node = 0;

    if(!root){
        root = ml_new_section();
        root->name = "root";
    }

    return root;
}