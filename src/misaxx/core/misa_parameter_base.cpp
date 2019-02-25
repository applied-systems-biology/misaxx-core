//
// Created by ruman on 25.02.19.
//

#include <misaxx/core/misa_parameter_base.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

misa_parameter_base::misa_parameter_base(misa_parameter_base::path t_location,
                                         std::shared_ptr<misa_json_schema_property> t_schema) : location(std::move(t_location)), schema(std::move(t_schema)) {

}