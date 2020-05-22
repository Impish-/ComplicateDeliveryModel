//
// Created by Соколов Илья on 23.05.2020.
//

#ifndef MDILIVERY_PRODUCT_H
#define MDILIVERY_PRODUCT_H

#include <iostream>
#include "../json.hpp"

using nlohmann::json;

namespace ns {
    struct product {
        int id;
        std::string name;
        std::string description;
    };

    void to_json(json& j, const product& p) {
        j = json{{"id", p.id}, {"name", p.name}, {"description", p.description}};
    }

    void from_json(const json& j, product& p) {
        j.at("id").get_to(p.id);
        j.at("name").get_to(p.name);
        j.at("description").get_to(p.description);
    }
} // namespace ns

#endif //MDILIVERY_PRODUCT_H
