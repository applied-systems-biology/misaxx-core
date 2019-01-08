#include <misaxx/filesystem/misa_filesystem_entry.h>

using namespace misaxx;

misa_filesystem_entry::misa_filesystem_entry(std::string t_name, misa_filesystem_entry_type t_type,
                                             misa_filesystem_entry::path t_custom_external) :
        name(std::move(t_name)), type(t_type), custom_external(std::move(t_custom_external)) {
}

misa_filesystem_entry::path misa_filesystem_entry::internal_path() const {
    if (parent.expired()) {
        return name;
    } else {
        return parent.lock()->internal_path() / name;
    }
}

bool misa_filesystem_entry::has_external_path() const {
    return !external_path().empty();
}

misa_filesystem_entry::path misa_filesystem_entry::external_path() const {
    if (!custom_external.empty())
        return custom_external;
    else if (!parent.expired())
        return parent.lock()->external_path() / name;
    else
        throw std::runtime_error("Path " + internal_path().string() + " has no external path!");
}

filesystem::entry misa_filesystem_entry::self() {
    return shared_from_this();
}

filesystem::const_entry misa_filesystem_entry::self() const {
    return shared_from_this();
}

filesystem::entry misa_filesystem_entry::create(std::string t_name, misa_filesystem_entry::path t_custom_external) {
    return insert(std::make_shared<misa_filesystem_entry>(std::move(t_name), type, std::move(t_custom_external)));
}

filesystem::entry misa_filesystem_entry::insert(filesystem::entry ptr) {
    ptr->parent = self();
    children.insert({ptr->name, ptr});
    return ptr;
}

misa_filesystem_entry::iterator misa_filesystem_entry::begin() {
    return children.begin();
}

misa_filesystem_entry::const_iterator misa_filesystem_entry::begin() const {
    return children.begin();
}

misa_filesystem_entry::iterator misa_filesystem_entry::end() {
    return children.end();
}

misa_filesystem_entry::const_iterator misa_filesystem_entry::end() const {
    return children.end();
}

misa_filesystem_entry::iterator misa_filesystem_entry::find(const std::string &t_name) {
    return children.find(t_name);
}

misa_filesystem_entry::const_iterator misa_filesystem_entry::find(const std::string &t_name) const {
    return children.find(t_name);
}

bool misa_filesystem_entry::empty() const {
    return children.empty();
}

void misa_filesystem_entry::ensure_external_path_exists() const {
    if (!has_external_path())
        throw std::runtime_error("This VFS folder has no external path!");
    if (!boost::filesystem::exists(external_path()))
        boost::filesystem::create_directories(external_path());
}

filesystem::entry misa_filesystem_entry::access(boost::filesystem::path t_segment) {
    t_segment.remove_trailing_separator();
    if (t_segment.empty())
        return self();

    misa_filesystem_entry *current = this;

    // Navigate to subfolders if needed
    for (const auto &seg : t_segment.parent_path()) {
        auto it = current->find(seg.string());
        if (it == end()) {
            current = create(seg.string()).get();
        } else {
            current = it->second.get();
        }
    }

    // Create / access the target element
    auto it = current->find(t_segment.filename().string());
    if (it == end()) {
        return create(t_segment.filename().string());
    } else {
        return it->second;
    }
}

filesystem::const_entry misa_filesystem_entry::at(boost::filesystem::path t_segment) const {
    t_segment.remove_trailing_separator();
    if (t_segment.empty())
        return self();

    const misa_filesystem_entry *current = this;
    auto current_segment_it = t_segment.begin();

    // Navigate to subfolders if needed
    for (const auto &seg : t_segment.parent_path()) {
        auto it = current->find(seg.string());
        if (it == end()) {
            throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
        } else {
            current = it->second.get();
        }
    }

    // Create / access the target element
    auto it = current->find(t_segment.filename().string());
    if (it == end()) {
        throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
    } else {
        return it->second;
    }
}

bool misa_filesystem_entry::has_subpath(boost::filesystem::path t_segment) const {
    t_segment.remove_trailing_separator();
    if (t_segment.empty())
        return true;

    const misa_filesystem_entry *current = this;
    auto current_segment_it = t_segment.begin();

    // Navigate to subfolders if needed
    for (const auto &seg : t_segment.parent_path()) {
        auto it = current->find(seg.string());
        if (it == end()) {
            return false;
        } else {
            current = it->second.get();
        }
    }

    // Create / access the target element
    auto it = current->find(t_segment.filename().string());
    return !(it == end());
}

void misa_filesystem_entry::from_json(const nlohmann::json &) {
    throw std::runtime_error("Not implemented");
}

void misa_filesystem_entry::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
    throw std::runtime_error("Not implemented");
}

void misa_filesystem_entry::to_json_schema(const misa_json_schema &t_schema) const {
    misa_serializeable::to_json_schema(t_schema);
    t_schema.resolve("external-path").declare_optional<std::string>();
    for(const auto &kv : children) {
        kv.second->to_json_schema(t_schema.resolve("children", kv.first));
    }
    metadata->to_json_schema(t_schema.resolve("metadata"));
}

boost::filesystem::path misa_filesystem_entry::child_external_path(const boost::filesystem::path &t_path) {
    if(t_path == external_path()) {
        return "/";
    }
    else {
        boost::filesystem::path absolute_path = boost::filesystem::absolute(t_path, boost::filesystem::current_path());
        boost::filesystem::path absolute_external_path = boost::filesystem::absolute(external_path(), boost::filesystem::current_path());
        if(boost::starts_with(absolute_path.string(), absolute_external_path.string())) {
            return boost::filesystem::relative(absolute_path, absolute_external_path);
        }
        else {
            return "";
        }
    }
}

void misa_filesystem_entry::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "filesystem/entry"));
}
