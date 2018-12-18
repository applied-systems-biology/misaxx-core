#include <misaxx/patterns/misa_image_file_pattern.h>

using namespace misaxx;

misa_image_file_pattern::misa_image_file_pattern() : misa_file_pattern({ ".tif", ".tiff", ".png",
                                                                         ".bmp", ".pbm", ".pgm",
                                                                         ".ppm", ".jpeg", ".jpg",
                                                                         ".jpe", ".jp2" }) {

}

void misa_image_file_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_file_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/image-file"));
}
