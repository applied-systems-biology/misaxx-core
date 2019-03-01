#include <misaxx/core/caches/misa_exported_attachments_cache.h>
#include <iomanip>

using namespace misaxx;

nlohmann::json &misa_exported_attachments_cache::get() {
    return m_json;
}

const nlohmann::json &misa_exported_attachments_cache::get() const {
    return m_json;
}

void misa_exported_attachments_cache::set(nlohmann::json value) {
    m_json = std::move(value);
}

bool misa_exported_attachments_cache::has() const {
    return false;
}

bool misa_exported_attachments_cache::can_pull() const {
    return boost::filesystem::is_regular_file(m_path);
}

void misa_exported_attachments_cache::pull() {
    std::ifstream sw;
    sw.open(m_path.string());
    sw >> m_json;
}

void misa_exported_attachments_cache::stash() {
    m_json = nlohmann::json::object();
}

void misa_exported_attachments_cache::push() {
    std::ofstream sw;
    sw.open(m_path.string());
    sw << std::setw(4) << m_json;
}

void misa_exported_attachments_cache::do_link(const misa_exported_attachments_description &t_description) {
    if(t_description.filename.empty())
        throw std::runtime_error("Cannot link to file description with empty file name!");
    m_path = this->get_location() / t_description.filename;
    this->set_unique_location(m_path);
}

void misa_exported_attachments_cache::save_attachments() {
    misaxx::utils::readonly_access<attachment_type > access(attachments);
    for(const auto &kv : access.get()) {
        const misa_serializable *md = kv.second.get();
        md->to_json(m_json[md->get_serialization_id().get_id()]);
    }
}

void misa_exported_attachments_cache::postprocess() {
    save_attachments();
    push();
}

void misa_exported_attachments_cache::simulate_link() {
    describe()->access<misa_json_pattern>();
    describe()->access<misa_exported_attachments_description>();
}

misa_exported_attachments_description
misa_exported_attachments_cache::produce_description(const boost::filesystem::path &, const misa_json_pattern &) {
    return misa_exported_attachments_description();
}
