#pragma once

#include <ostream>
#include <string>

#include <ml.hpp>

std::string att_to_txt(const dtk::ml::MLAttribute* a);

void ml_to_txt(std::ostream& out, const dtk::ml::MLRoot* root);
void ml_to_xml(std::ostream& out, const dtk::ml::MLRoot* root);