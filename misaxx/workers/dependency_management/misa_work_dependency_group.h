//
// Created by rgerst on 01.08.18.
//


#pragma once

#include <misaxx/workers/misa_worker_base.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <misaxx/workers/dependency_management/misa_work_dependency_segment.h>
#include <misaxx-helpers/type_traits.h>

namespace misaxx::dependencies {

    /**
     * A group is an organization of workers to allow another worker or another group of workers to depend on them.
     */
    class misa_work_dependency_group : public misa_work_dependency_segment {

    public:

        misa_work_dependency_group() = default;

        misa_work_dependency_group(const misa_work_dependency_group&t_other) = delete;

        misa_work_dependency_group(const std::initializer_list<std::reference_wrapper<misa_work_dependency_segment>> &t_segments);

        /**
         * Creates a singleton group from a worker instance.
         * The worker is assumed to have no additional dependencies.
         * @param t_worker
         */
        explicit misa_work_dependency_group(misa_worker_base &t_worker);

        depencencies_t dependencies() const override;

        depencencies_t to_dependencies() override;

        void assign(std::shared_ptr<nodes::misa_work_node> t_node);

        /**
         * Adds another chain/group as dependency. Please note that if any node was assigned to this group,
         * the method will throw an exception.
         * @param t_segment
         */
        void add_dependency(misa_work_dependency_segment &t_segment);

        /**
         * Either inserts a worker or another group/chain into the group.
         *
         * If a worker is inserted into the group, it is added to the list of dependencies that will be returned on to_dependencies().
         * Please note that this structure will assume that the instance is assumed to have no dependencies.
         *
         * If another group or chain is inserted, their to_dependencies() results will be added to the dependency list of this group.         *
         * Please note that adding another group/chain is only permitted if no worker was added before.
         *
         * @tparam InstanceOrSegment
         * @param t_instance
         * @return
         */
        template<class InstanceOrSegment> misa_work_dependency_group &operator << (InstanceOrSegment &t_instance) {
            if constexpr (std::is_base_of<misa_worker_base, InstanceOrSegment>::value) {
                assign(t_instance.get_node());
            }
            else if constexpr (std::is_base_of<misa_work_dependency_segment, InstanceOrSegment>::value) {
                add_dependency(t_instance);
            }
            else {
                static_assert(cxxh::always_false<InstanceOrSegment>::value, "Invalid type!");
            }
            return *this;
        }

        /**
         * Applies the dependencies to the instance, which is equivalent to creating a new group with this one as
         * dependency and dispatching a worker onto it.
         * Please note that this operator will lock the group and prevent further insertions.
         *
         * \code{.cpp}
         *
         * group my_group;
         *
         * // Canonical way
         * group({{ my_group }}) << dispatch<my_task>("important task");
         *
         * // Alternative way
         * my_group >> dispatch<my_task>("important task");
         *
         * \endcode
         * @tparam Instance
         * @param t_instance
         * @return
         */
        template<class Instance> misa_work_dependency_group &operator >> (Instance &t_instance) {
            static_assert(std::is_base_of<misa_worker_base, Instance>::value, "Only workers can be inserted!");
            t_instance.get_node().get_dependencies() = to_dependencies();
            return *this;
        }

    private:

        bool m_dependency_locked = false;
        bool m_locked = false;
        depencencies_t m_dependencies;
        depencencies_t m_as_dependencies;

    };

}