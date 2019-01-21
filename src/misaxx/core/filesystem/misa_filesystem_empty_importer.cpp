#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>

using namespace misaxx;

misa_filesystem misa_filesystem_empty_importer::import() {
    misa_filesystem vfs;
    vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type::imported);
    vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type::exported);
    return vfs;
}
