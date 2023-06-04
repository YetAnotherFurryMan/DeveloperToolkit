#include "../headers/converters.hpp"

#include <sstream>

std::string sec_to_txt(const dtk::ml::MLSection* s, const std::string& prefix){
    std::stringstream ss;
    ss << prefix << s->name;
    
    std::string _prefix = prefix + "\t";

    //Write mods:
    for(size_t i = 0; i < s->modifier_no; i++)
        ss << " " << att_to_txt(s->modifiers[i]);
    
    ss << ":" << std::endl;

    //Write defs:
    for(size_t i = 0; i < s->definition_no; i++)
        ss << _prefix << "define " << att_to_txt(s->definitions[i]) << std::endl;
    
    //Write atts:
    for(size_t i = 0; i < s->attribute_no; i++)
        ss << _prefix << "define " << att_to_txt(s->attributes[i]) << std::endl;
    
    //Write secs:
    for(size_t i = 0; i < s->section_no; i++)
        ss << sec_to_txt(s->sections[i], _prefix) << std::endl;
    
    //Write vals:
    for(size_t i = 0; i < s->value_no; i++)
        ss << _prefix << "\"" << std::string(dtk::ml::ml_format_str(s->values[i])) << "\"" << std::endl;
    
    return ss.str();
}

void ml_to_txt(std::ostream& out, const dtk::ml::MLRoot* root){
    //Write defs:
    for(size_t i = 0; i < root->definition_no; i++)
        out << "define " << att_to_txt(root->definitions[i]) << std::endl;
    
    out << std::endl;
    
    //Write atts:
    for(size_t i = 0; i < root->attribute_no; i++)
        out << "define " << att_to_txt(root->attributes[i]) << std::endl;

    out << std::endl;
    
    //Write secs:
    for(size_t i = 0; i < root->section_no; i++)
        out << sec_to_txt(root->sections[i], "") << std::endl;
}