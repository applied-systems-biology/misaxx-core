/**
 * Copyright by Ruman Gerst
 * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
 * https://www.leibniz-hki.de/en/applied-systems-biology.html
 * HKI-Center for Systems Biology of Infection
 * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
 * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
 *
 * This code is licensed under BSD 2-Clause
 * See the LICENSE file provided with this code for the full license.
 */

#pragma once

namespace misaxx {
    /**
     * Status of a node
     */
    enum class misa_worker_status : int {
        /**
         * Work is not done
         */
        undone = 0,
        /**
         * Worker is ready and waiting for a thread
         */
        ready = 1,
        /**
         * The task is currently working.
         */
        working = 2,
        /**
         * The work is done
         */
        done = 3,
        /**
         * The worker rejected the work (e.g. via worker::input<>::exists).
         * Work must be retried later
         */
        queued_repeat = 4,
        /**
         * The worker is waiting for sub-workers
         */
        waiting = 5
    };

}