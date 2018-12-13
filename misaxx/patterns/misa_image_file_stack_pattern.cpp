#include <misaxx/patterns/misa_image_file_stack_pattern.h>

using namespace misaxx;

misa_image_file_stack_pattern::misa_image_file_stack_pattern() : misa_file_stack_pattern({ ".tif", ".tiff", ".png",
                                                                                           ".bmp", ".pbm", ".pgm",
                                                                                           ".ppm", ".jpeg", ".jpg",
                                                                                           ".jpe", ".jp2" }) {

}

void misa_image_file_stack_pattern::from_json(const nlohmann::json &t_json) {
    if(t_json.find("extensions") != t_json.end())
        misa_file_stack_pattern::from_json(t_json);
}

void misa_image_file_stack_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_file_stack_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/image-file-stack"));
}
