%{
    #include "../headers/DTKDS.h"

    #include <malloc.h>
    #include <string.h>

    struct DSRoot root_node = {0,0,0,0,0,0};

    struct DSRoot* ds_get_root();

    void ds_add_definition(struct DSSection* s, struct DSAttribute* d);
    void ds_add_attribute(struct DSSection* s, struct DSAttribute* a);
    void ds_add_modifier(struct DSSection* s, struct DSAttribute* m);
    void ds_add_section(struct DSSection* s, struct DSSection* ss);
    void ds_add_value(struct DSSection* s, char* v);

    struct DSAttribute* ds_new_attribute(char* name, char* value);
%}

%define api.prefix {ds_yy}

%define api.value.type union

%token <char*> TK_ID
%token <char*> TK_VALUE

%type <struct DSSection*> section;
%type <struct { size_t no; struct DSAttribute** mods; }> section_modifiers;
%type <struct DSSection*> section_in;

%type <struct DSAttribute*> attribute_value;

%type <struct DSAttribute*> definition;
%type <struct DSAttribute*> attribute;
%type <struct DSAttribute*> modifier;

%%

root:
      root section {
        root_node.section_no++;

        if(root_node.sections == 0)
            root_node.sections = malloc(sizeof(struct DSSection*));
        else
            root_node.sections = realloc(root_node.sections, (sizeof(struct DSSection*)) * root_node.section_no);
        
        root_node.sections[root_node.section_no - 1] = $2;
      }
    | root attribute {
        root_node.attribute_no++;

        if(root_node.attributes == 0)
            root_node.attributes = malloc(sizeof(struct DSAttribute*));
        else 
            root_node.attributes = realloc(root_node.attributes, (sizeof(struct DSAttribute*)) * root_node.attribute_no);
        
        root_node.attributes[root_node.attribute_no - 1] = $2;
      }
    | root definition {
        root_node.definition_no++;

        if(root_node.definitions == 0)
            root_node.definitions = malloc(sizeof(struct DSAttribute*));
        else 
            root_node.definitions = realloc(root_node.definitions, (sizeof(struct DSAttribute*)) * root_node.definition_no);
        
        root_node.definitions[root_node.definition_no - 1] = $2;
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
        ds_add_section($$, $2);
      }
    | section_in attribute {
        $$ = $1;
        ds_add_attribute($$, $2);
      }
    | section_in definition {
        $$ = $1;
        ds_add_definition($$, $2);
      }
    | section_in TK_VALUE {
        $$ = $1;
        ds_add_value($$, $2);
      }
    | %empty { $$ = malloc(sizeof(struct DSSection)); }
;

is: '=' | ':';

attribute_value:
      TK_ID is TK_VALUE {
        $$ = ds_new_attribute($1, $3);
      }
    | TK_ID {
        $$ = ds_new_attribute($1, 0);
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
        $$ = ds_new_attribute($1, $3);
      }
    | TK_ID {
        $$ = ds_new_attribute($1, 0);
    }
;

%%

struct DSRoot* ds_get_root(){
    return &root_node;
}

void ds_add_definition(struct DSSection* s, struct DSAttribute* d){
    s->definition_no++;

    if(s->definitions == 0)
        s->definitions = malloc(sizeof(struct DSAttribute*));
    else
        s->definitions = realloc(s->definitions, sizeof(struct DSAttribute*) * s->definition_no);
    
    s->definitions[s->definition_no - 1] = d;
}

void ds_add_attribute(struct DSSection* s, struct DSAttribute* a){
    s->attribute_no++;

    if(s->attributes == 0)
        s->attributes = malloc(sizeof(struct DSAttribute*));
    else
        s->attributes = realloc(s->attributes, sizeof(struct DSAttribute*) * s->attribute_no);
    
    s->attributes[s->attribute_no - 1] = a;
}

void ds_add_modifier(struct DSSection* s, struct DSAttribute* m){
    s->modifier_no++;

    if(s->modifiers == 0)
        s->modifiers = malloc(sizeof(struct DSAttribute*));
    else
        s->modifiers = realloc(s->modifiers, sizeof(struct DSAttribute*) * s->modifier_no);
    
    s->modifiers[s->modifier_no - 1] = m;
}

void ds_add_section(struct DSSection* s, struct DSSection* ss){
    s->section_no++;

    if(s->sections == 0)
        s->sections = malloc(sizeof(struct DSSection*));
    else
        s->sections = realloc(s->sections, sizeof(struct DSSection*) * s->section_no);
    
    s->sections[s->section_no - 1] = ss;
}

void ds_add_value(struct DSSection* s, char* v){
    s->value_no++;

    if(s->values == 0)
        s->values = malloc(sizeof(char*));
    else
        s->values = realloc(s->values, sizeof(char*) * s->value_no);
    
    s->values[s->value_no - 1] = v;
}

struct DSAttribute* ds_new_attribute(char* name, char* value){
    struct DSAttribute* a = malloc(sizeof(struct DSAttribute));

    a->name = malloc(strlen(name));
    strcpy(a->name, name);

    if(value){
        a->value = malloc(strlen(value));
        strcpy(a->value, value);
    } else{
        a->value = 0;
    }

    return a;
}