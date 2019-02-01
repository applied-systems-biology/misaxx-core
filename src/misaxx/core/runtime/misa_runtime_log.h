//
// Created by rgerst on 01.02.19.
//

#pragma once
#include <misaxx/core/workers/misa_work_node.h>
#include <chrono>
#include <misaxx/core/misa_serializable.h>
#include <mutex>

namespace misaxx {
    class misa_runtime_log : public misa_serializable {
    public:
        using clock = std::chrono::high_resolution_clock;
        using time_point = std::chrono::time_point<clock>;
        using duration = std::chrono::duration<double, std::milli >;

        misa_runtime_log() = default;

        struct entry : public misa_serializable {
            const misa_runtime_log *m_log = nullptr;
            std::string name;
            time_point start_time;
            time_point end_time;

            entry() = default;

            explicit entry(const misa_runtime_log &log);

            void from_json(const nlohmann::json &t_json) override;

            void to_json(nlohmann::json &t_json) const override;

            void to_json_schema(const misa_json_schema &t_schema) const override;

        protected:
            void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
        };

        void start(uintptr_t id, std::string name);

        void stop(uintptr_t id);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    private:
        std::mutex mutex;
        time_point start_time = clock::now();
        std::unordered_map<uintptr_t, entry> entries;
    };

    inline void to_json(nlohmann::json& j, const misa_runtime_log& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_runtime_log& p) {
        p.from_json(j);
    }

    inline void to_json(nlohmann::json& j, const misa_runtime_log::entry& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_runtime_log::entry& p) {
        p.from_json(j);
    }
}




