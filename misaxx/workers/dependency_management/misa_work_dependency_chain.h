//
// Created by rgerst on 01.08.18.
//


#pragma once

#include <misaxx/workers/misa_worker_base.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include "misa_work_dependency_segment.h"

namespace misaxx::dependencies {

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

        misa_work_dependency_chain(const std::initializer_list<std::reference_wrapper<misa_work_dependency_segment>> &t_segments) {
            for (const auto &seg : t_segments) {
                for (auto &nd : seg.get().to_dependencies()) {
                    m_dependencies.insert(nd);
                }
            }
            m_as_dependencies = m_dependencies;
            m_consecutive_dependencies = m_dependencies;
        }

        depencencies_t dependencies() const override {
            return m_dependencies;
        }

        depencencies_t to_dependencies() override {
            m_locked = true;
            return m_as_dependencies;
        }

        void assign(std::shared_ptr<nodes::misa_work_node> t_node) {
            if (m_locked) {
                throw std::runtime_error("Cannot assign nodes to this chain after it has been used as dependency!");
            }
            m_as_dependencies.insert(t_node);
            t_node->get_dependencies() = m_consecutive_dependencies;
            m_consecutive_dependencies.insert(std::move(t_node));
        }

        /**
         * Inserts the dependencies of given segment into the chain.
         * The dependencies apply to the next inserted worker and the following workers.
         * In contrast to a misaxx::dependencies::group, there are no restrictions.
         * @param t_segment
         */
        void add_dependency(misa_work_dependency_segment &t_segment) {
            for (auto &nd : t_segment.to_dependencies()) {
                m_consecutive_dependencies.insert(nd);
            }
        }

        template<class InstanceOrSegment>
        misa_work_dependency_chain &operator>>(InstanceOrSegment &t_instance) {
            if constexpr (std::is_base_of<misaxx::misa_worker_base, InstanceOrSegment>::value) {
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