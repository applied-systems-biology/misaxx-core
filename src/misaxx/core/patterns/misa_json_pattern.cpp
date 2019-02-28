//
// Created by rgerst on 26.02.19.
//

#include <misaxx/core/patterns/misa_json_pattern.h>

#include "misaxx/core/patterns/misa_json_pattern.h"

std::string misaxx::misa_json_pattern::get_documentation_name() const {
    return "JSON file pattern";
}

std::string misaxx::misa_json_pattern::get_documentation_description() const {
    return "Finds a JSON file";
}

misaxx::misa_json_pattern::misa_json_pattern() : misa_file_pattern({ ".json" }) {

}
