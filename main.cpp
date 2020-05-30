#include "classes/MapPoint.h"
#include "classes/Map.h"
#include <iostream>
#include <unistd.h>
#include <restinio/all.hpp>
#include <map>
#include <string>
using nlohmann::json;
using router_t = restinio::router::express_router_t<>;

void test_bad_order1(Map<RoadObject> & map){
    // test unreal delivery time
    try {
        std::map<string, int> orderProducts = {
                pair<string, int>("Экран", 2),
               pair<string, int>("Акустическая система", 2),
               pair<string, int>("Световое оформление", 2),
               pair<string, int>("Дым-машина", 2),
        };
        map.processOrder(13, 17, 10, orderProducts);
    } catch (CantDeliveryException& ex) {
        cout << ex.PrintMessage();
        throw ex;
    }
}
void test_bad_order2(Map<RoadObject> & map){
    // test bad product
    try {
        std::map<string, int> orderProducts = {pair<string, int>("Экран", 2),
                pair<string, int>("Акустическая система", 2),
                pair<string, int>("Световое оформление", 2),
                pair<string, int>("Дым-машина", 2),
                pair<string, int>("Неведомое совершенно", 2),
        };
        map.processOrder(13, 17, 10, orderProducts);
    } catch (CantDeliveryException& ex) {
        cout << ex.PrintMessage();
        throw ex;
    }
}

int test_road(Map<RoadObject> & map){
//    map.insert(10, 15);
    map.insert(11, 17);
    map.insert(12, 16);
    map.insert(12, 17);
    map.insert(13, 17);
};

std::map<string, int> getDefaultProducts(){
    std::map<string, int> defaultProducts = {
            pair<string, int>("Экран", 2),
            pair<string, int>("Сцена", 2),
            pair<string, int>("Акустическая система", 2),
            pair<string, int>("DJ оборудование", 2),
            pair<string, int>("Световое оформление", 2),
            pair<string, int>("Баннер", 2),
            pair<string, int>("Подиум", 2),
            pair<string, int>("Дым-машина", 2),
            pair<string, int>("Конфетти-машина", 2),
            pair<string, int>("Радио-микрофон", 2),
            pair<string, int>("Плазма", 2),
            pair<string, int>("Ферма сценическа", 2),
            pair<string, int>("Ферма Основание для ферм", 2),
    };
    return defaultProducts;
}

int test_initials(Map<RoadObject> & map) {
    //test insert
    std::map<string, int>  a = {
            pair<string, int>("Экран", 3),
            pair<string, int>("Сцена", 1),
            pair<string, int>("DJ оборудование", 2),
            pair<string, int>("Подиум", 2),
            pair<string, int>("Конфетти-машина", 1),
            pair<string, int>("Радио-микрофон", 3),
    };
    map.addStore(10, 15, "TestStore", a);
    a = {

            pair<string, int>("Световое оформление", 4),
            pair<string, int>("Подиум", 1),
            pair<string, int>("Дым-машина", 5),
            pair<string, int>("Конфетти-машина", 4),
            pair<string, int>("Ферма сценическа", 2),
            pair<string, int>("Ферма Основание для ферм", 2),
    };
    map.addStore(19, 50, "TestStore2", a);

    a = {
            pair<string, int>("DJ оборудование", 1),
            pair<string, int>("Дым-машина", 1),
            pair<string, int>("Конфетти-машина", 4),
            pair<string, int>("Радио-микрофон", 2),
            pair<string, int>("Плазма", 2),
    };
    map.addStore(2, 47, "TestStore3", a);

    a = {
            pair<string, int>("DJ оборудование", 1),
            pair<string, int>("Баннер", 5),
            pair<string, int>("Конфетти-машина", 5),
    };
    map.addStore(4, 25, "TestStore4", a);
};

void test_order(Map<RoadObject> & map){
    //    std::map<string, int> orderProducts = {
//            pair<string, int>("Экран", 1),
//            pair<string, int>("Сцена", 1),
//            pair<string, int>("Баннер", 1),
//            pair<string, int>("DJ оборудование", 1),
//            pair<string, int>("Плазма", 1)
//    };

    std::map<string, int> orderProducts = {
            pair<string, int>("DJ оборудование", 4),
            pair<string, int>("Дым-машина", 3),
    };

    try {
        map.processOrder(13, 17, 64, orderProducts);
    } catch (CantDeliveryException& ex) {
        cout << ex.PrintMessage();
    }

}

using my_router_t = restinio::router::express_router_t<>;

auto make_request_handler(){
    Map<RoadObject> * map = new Map<RoadObject>(20,50);
    test_initials(*map); //stores
    test_road(*map);
//    test_order(map);
    int i=0;

    auto router = std::make_unique<my_router_t>();

    router->http_get(R"(/)",
                      [map, i](auto req, auto params) mutable {
                          map->nextTick(i++);
                          string body = map->serialize().dump();
                          return req->create_response()
                                    .set_body(std::move( body ))
                                    .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                    .append_header("Access-Control-Allow-Origin", "*")
                                    .done();
                      });

        router->http_post(R"(/order)",
                     [map](auto req, auto params) mutable {
                         std::string s = req->body();
                         json j2 =  json::parse(s);
                         string body = "OK";
                         std::map<string, int> orderProducts;

                         for (auto x : j2["products"]){
                             orderProducts[x["name"]] = x["count"];
                         }
                         try{
                             map->processOrder(j2["point"]["x"], j2["point"]["y"], j2["deliveryTick"], orderProducts);
                         } catch (CantDeliveryException& ex) {
                             body = "FAIL";
                         }

                         return req->create_response()
                                 .set_body(std::move( body ))
                                 .done();
                     });

    router->http_post(R"(/store)",
                      [map](auto req, auto params) mutable {
                          std::string s = req->body();
                          json j2 =  json::parse(s);
                          string body = "OK";
                          std::map<string, int> storeProducts;

                          for (auto x : j2["products"]){
                              storeProducts[x["name"]] = x["count"];
                          }
                          try{
                              map->addStore(j2["point"]["x"], j2["point"]["y"], j2["name"], storeProducts);;
                          } catch (...) {

                          }

                          return req->create_response()
                                  .set_body(std::move( body ))
                                  .done();
                      });

    return router;
}

int main() {
    struct my_traits : public restinio::default_single_thread_traits_t {
        using request_handler_t = my_router_t;
    };

    restinio::run(
            restinio::on_this_thread<my_traits>()
                    .address("localhost")
                    .port(8080)
                    .request_handler(make_request_handler())
    );
    return 0;
}