#include <misaxx/workers/misa_task.h>

using namespace misaxx;

void misa_task::simulate_work() {
}

void misa_task::execute_work() {
    if(misaxx::misa_runtime_base::instance().is_simulating())
        simulate_work();
    else
        work();
}

bool misa_task::is_parallelizeable() const {
    return m_is_parallelizeable;
}
