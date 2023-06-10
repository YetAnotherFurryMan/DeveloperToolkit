#include "../headers/converters.hpp"

#include <sstream>

#include <common.hpp>

std::string val_to_xml(char* s){
    std::string str = dtk::ml::ml_format_str(s);
    dtk::common::str::replase_all(str, "&", "&amp;");
    dtk::common::str::replase_all(str, "<", "&lt;");
    dtk::common::str::replase_all(str, ">", "&gt;");
    dtk::common::str::replase_all(str, "\\\'", "&apos;");
    dtk::common::str::replase_all(str, "\\\"", "&quot;");
    return str;
}

std::string att_to_xml(const dtk::ml::MLAttribute* a){
    std::string xml = "";
    xml += a->name;
    xml += "=\"";
    if(a->value)
        xml += val_to_xml(a->value);
    xml += "\"";
    return xml;
}

std::string sec_to_xml(const dtk::ml::MLSection* s, const std::string& prefix){
    std::stringstream ss;
    ss << prefix << "<" << s->name;

    //Definitions
    for(size_t i = 0; i < s->definition_no; i++)
        ss << " def." << att_to_xml(s->definitions[i]);
    
    //Modifiers
    for(size_t i = 0; i < s->modifier_no; i++)
        ss << " mod." << att_to_xml(s->modifiers[i]);

    ss << ">" << std::endl;
    
    std::string _prefix = prefix + "\t";

    //Attributes
    for(size_t i = 0; i < s->attribute_no; i++)
        ss << _prefix << "<att." << s->attributes[i]->name << ">" << (s->attributes[i]->value?val_to_xml(s->attributes[i]->value):"") << "</att." << s->attributes[i]->name << ">" << std::endl;
    
    //Sections
    for(size_t i = 0; i < s->section_no; i++)
        ss << sec_to_xml(s->sections[i], _prefix) << std::endl;
    
    //Values
    for(size_t i = 0; i < s->value_no; i++)
        ss << _prefix << "<value>" << val_to_xml(s->values[i]) << "</value>" << std::endl;
    
    ss << prefix << "</" << s->name << ">";

    return ss.str();
}

void ml_to_xml(std::ostream& out, const dtk::ml::MLRoot* root){
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    out << "<root";
    
    //Definitions
    for(size_t i = 0; i < root->definition_no; i++)
        out << " def." << att_to_xml(root->definitions[i]);
    
    //Attributes
    for(size_t i = 0; i < root->attribute_no; i++)
        out << " att." << att_to_xml(root->attributes[i]);
    
    out << ">" << std::endl; //End of root

    //Sections
    for(size_t i = 0; i < root->section_no; i++)
        out << sec_to_xml(root->sections[i], "\t") << std::endl;
    
    out << "</root>" << std::endl;
}