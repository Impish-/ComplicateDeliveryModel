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

auto make_request_handler(Map<RoadObject> * map){
    auto router = std::make_unique<my_router_t>();

    router->http_get(R"(/tick)",
                      [map](auto req, auto params) mutable {
                          string body = map->jsonState;
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

                         for (json x : j2["products"]){
                             auto count = x["count"].get<int>();
                             if (count > 50) {
                                 json err;
                                 err["error"] = "too many items";
                                 return req->create_response(400, "Bad Request")
                                         .set_body(std::move( err.dump() ))
                                         .done();
                             }
                             orderProducts[x["name"].get<string>()] = count;
                         }
                         try{
                             map->processOrder(j2["point"]["x"].get<int>(), j2["point"]["y"].get<int>(),
                                     j2["deliveryTick"].get<int>(), orderProducts);
                         } catch (PastIntervalException& ex) {
                             json err;
                             err["error"] = "too late";
                             return req->create_response(400, "Bad Request")
                                     .set_body(std::move( err.dump() ))
                                     .done();
                         }

                         return req->create_response()
                                 .set_body(std::move( body ))
                                 .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                 .append_header("Access-Control-Allow-Origin", "*")
                                 .done();
                     });

                    router->http_post(R"(/stopStartWorld)",
                      [map](auto req, auto params) mutable {
                          map->runLoop = !map->runLoop;
                          string body = (map->runLoop)? "started": "stopped";

                          return req->create_response()
                                  .set_body(std::move( body ))
                                  .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                  .append_header("Access-Control-Allow-Origin", "*")
                                  .done();
                      });

                router->http_post(R"(/store)",
                                  [map](auto req, auto params) mutable {
                                      std::string s = req->body();
                                      const auto token = req->header().get_field(
                                              "auth" );

                                      if (token !="q1wefsa^%)"){
                                          return req->create_response(403, "No Accept")
                                                  .done();
                                      }

                                      cout << s <<endl;
                                      json j2 =  json::parse(s);
                                      string body = "OK";
                                      std::map<string, int> storeProducts;

                                      for (json x : j2["products"]){
                                          storeProducts[x["name"].get<string>()] = x.at("count").get<int>();
                                      }
                                      try{
                                          map->addStore(j2["point"]["x"].get<int>(),
                                                  j2["point"]["y"].get<int>(), j2["name"].get<string>(), storeProducts);;
                                      } catch (...) {

                                      }

                                      return req->create_response()
                                              .set_body(std::move( body ))
                                              .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                              .append_header("Access-Control-Allow-Origin", "*")
                                              .append_header("Access-Control-Allow-Headers", "*")
                                              .done();
                                  });


    router->http_post(R"(/login)",
                      [map](auto req, auto params) mutable {
                          std::string s = req->body();
                          json j2 =  json::parse(s);
                          string body = "";
                          if (j2["login"].get<string>() == "admin" & j2["password"].get<string>() == "admin"){
                              json resp;
                              resp["token"] = "q1wefsa^%)";
                              body = resp.dump();
                          }

                          return req->create_response()
                                  .set_body(std::move( body ))
                                  .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                  .append_header("Access-Control-Allow-Origin", "*")
                                  .append_header("Access-Control-Allow-Headers", "*")
                                  .done();
                      });

    router->http_get(R"(/)",
                      [map](auto req, auto params) mutable {
                          auto sf = restinio::sendfile( "cli.html");
                          return req->create_response()
                                  .set_body(std::move( sf ) )
//                                  .append_header( restinio::http_field::content_type, "text/plain; charset=utf-8")
                                  .append_header("Access-Control-Allow-Origin", "*")
                                  .append_header("Access-Control-Allow-Headers", "*")
                                  .done();
                      });
    router->non_matched_request_handler(
            []( auto req ){
                return
                        req->create_response( 200, "any")
                                .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                .append_header("Access-Control-Allow-Origin", "*")
                                .append_header("Access-Control-Allow-Headers", "*")
                                .done();
            } );


    return router;
}


int main() {
    Map<RoadObject> * map = new Map<RoadObject>(20,50);
    test_initials(*map); //stores
    test_road(*map);

    auto timer = [map]() mutable{
        auto runTimer = [map]() mutable{
            int i = map->getTick() ;
            i++;
            while (map->runLoop) {
                map->nextTick(i++);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            while (!map->runLoop){
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        };
        while(map->loaded){
            auto future = std::async(runTimer);
        };
    };
    auto future = std::async(timer);

    struct my_traits : public restinio::default_single_thread_traits_t {
        using request_handler_t = my_router_t;
    };
    restinio::run(
            restinio::on_this_thread<my_traits>()
                    .address("127.0.0.1")
                    .port(8080)
                    .request_handler(make_request_handler(map))
    );
    return 0;
}