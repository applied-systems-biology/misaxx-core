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
        queued_repeat,
        /**
         * The worker is waiting for sub-workers
         */
        waiting
    };

}