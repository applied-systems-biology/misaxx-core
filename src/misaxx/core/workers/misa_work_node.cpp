#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_dispatcher.h>
#include "misa_work_node_impl.h"

using namespace misaxx;

std::shared_ptr<misa_work_node>
misa_work_node::create_instance(const std::string &t_name, const std::shared_ptr<misa_work_node> &t_parent,
                                misa_work_node::instantiator_type t_instantiator) {
    return std::make_shared<misa_work_node_impl>(t_name, t_parent, std::move(t_instantiator));
}
