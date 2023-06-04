#include "../headers/converters.hpp"

#include <sstream>

std::string att_to_txt(const dtk::ml::MLAttribute* a){
    return std::string(a->name) + "=\"" + ((a->value)?(std::string(dtk::ml::ml_format_str(a->value))):"") + "\"";
}