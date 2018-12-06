//
// Created by rgerst on 05.12.18.
//

#pragma once

#include "misa_range.h"

namespace misaxx {
    /**
    * A voxel defined by its extents in X, Y and Z directions
    * @tparam T
    */
    template<typename T> struct misa_voxel : public misa_serializeable {
        misa_range<T> X;
        misa_range<T> Y;
        misa_range<T> Z;

        misa_voxel() = default;

        explicit misa_voxel(misa_range<T> x, misa_range<T> y, misa_range<T> z) :
                X(std::move(x)), Y(std::move(y)), Z(std::move(z)) {

        }

        /**
         * Initializes the voxel with X = [0, x); Y = [0, y); Z = [0, z)
         * @param x
         * @param y
         * @param z
         */
        explicit misa_voxel(T x, T y, T z) :
        X(misa_range<T>(T(), x)), Y(misa_range<T>(T(), y)), Z(misa_range<T>(T(), z)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            X.from_json(t_json["x"]);
            Y.from_json(t_json["y"]);
            Z.from_json(t_json["z"]);
        }

        void to_json(nlohmann::json &t_json) const override {
            X.to_json(t_json["x"]);
            Y.to_json(t_json["y"]);
            Z.to_json(t_json["z"]);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            X.to_json_schema(t_schema.resolve("x"));
            Y.to_json_schema(t_schema.resolve("y"));
            Z.to_json_schema(t_schema.resolve("z"));
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa_ome", "attachments/cube");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return {get_serialization_id()};
        }

        /**
         * Calculates the volume of the voxel
         * @return
         */
        T size() const {
            return X.size() * Y.size() * Z.size();
        }

        /**
         * Returns true if the voxel contains the coordinate
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool contains(const T &x, const T &y, const T &z) const {
            return X.contains(x) && Y.contains(y) && Z.contains(z);
        }

        /**
         * Updates the voxel, so it includes the provided point
         * @param x
         * @param y
         * @param z
         */
        void include(const T &x, const T &y, const T &z) {
            X.include(x);
            Y.include(y);
            Z.include(z);
        }
    };
}



