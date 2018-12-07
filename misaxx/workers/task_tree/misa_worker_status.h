//
// Created by rgerst on 31.07.18.
//


#pragma once

namespace misaxx::nodes {
    /**
     * Status of a node
     */
    enum class misa_worker_status : int {
        /**
         * Work is not done
         */
        undone,
        /**
         * The task is currently working.
         */
        working,
        /**
         * The work is done
         */
        done,
        /**
         * The worker rejected the work (e.g. via worker::input<>::exists).
         * Work must be retried later
         */
        rejected
    };

}