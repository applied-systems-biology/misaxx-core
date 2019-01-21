//
// Created by rgerst on 01.08.18.
//


#pragma once

#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/workers/misa_work_dependency_segment.h>

namespace misaxx {
    struct misa_worker;
}

namespace misaxx {

    /**
     * A chain builds a consecutive relationship between instances and other segments.
     * If worker instance is inserted into a chain, it will depend on all other workers that were inserted before it.
     * Chain supports insertion of other segments in-between the chain links additionally to the initial chain dependencies.
     * All inserted worker instances will then also depend on those additional dependencies.
     *
     * In contrast to a group, insertion is done using the >> operator to differentiate the different containers.
     */
    class misa_work_dependency_chain : public misa_work_dependency_segment {

    public:

        misa_work_dependency_chain() = default;

        misa_work_dependency_chain(const misa_work_dependency_chain &t_other) = delete;

        misa_work_dependency_chain(const std::initializer_list<std::reference_wrapper<misa_work_dependency_segment>> &t_segments);

        depencencies_t dependencies() const override;

        depencencies_t to_dependencies() override;

        void assign(std::shared_ptr<misa_work_node> t_node);

        /**
         * Inserts the dependencies of given segment into the chain.
         * The dependencies apply to the next inserted worker and the following workers.
         * In contrast to a misaxx::dependencies::group, there are no restrictions.
         * @param t_segment
         */
        void add_dependency(misa_work_dependency_segment &t_segment);

        template<class InstanceOrSegment>
        misa_work_dependency_chain &operator>>(InstanceOrSegment &t_instance) {
            if constexpr (std::is_base_of<misaxx::misa_worker, InstanceOrSegment>::value) {
                assign(t_instance.get_node());
            } else {
                assign(t_instance);
            }
            return *this;
        }

    private:

        bool m_locked = false;
        depencencies_t m_dependencies;
        depencencies_t m_consecutive_dependencies;
        depencencies_t m_as_dependencies;

    };

}