//
// Created by rgerst on 13.11.18.
//

#include <misaxx/core/misa_cached_data_base.h>

void misaxx::misa_cached_data_base::suggest_document_title(std::string title) {
    if(describe()->documentation_title.empty())
        describe()->documentation_title = std::move(title);
}

void misaxx::misa_cached_data_base::suggest_document_description(std::string description) {
    if(describe()->documentation_description.empty())
        describe()->documentation_description = std::move(description);
}
