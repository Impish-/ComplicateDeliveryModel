#include "classes/MapPoint.h"
#include "classes/Map.h"
#include "models/product.h"
#include <iostream>
#include <unistd.h>
#include <restinio/all.hpp>

using router_t = restinio::router::express_router_t<>;

int test_road(Map<RoadObject> & map){
//    map.insert(10, 15);
    map.insert(11, 15);
    map.insert(11, 16);
    map.insert(11, 17);
    map.insert(12, 16);
    map.insert(12, 17);
    map.insert(13, 17);
    map.insert(14, 17);
    map.insert(15, 17);
    map.insert(16, 17);
    map.insert(17, 17);
    map.insert(18, 17);
    map.insert(18, 18);
    map.insert(19, 16);
    map.insert(13, 15);
    map.insert(20, 15);

};

int test_initials(Map<RoadObject> & map) {
    //test insert
    list<int> a = {1, 2, 3, 4, 5};
    map.addStore(10, 15, "TestStore", a);

    a = {6, 7, 8, 9, 10};
    map.addStore(19, 50, "TestStore2", a);

    a = {6, 7, 8, 9, 10};
    map.addStore(2, 47, "TestStore3", a);
}

int main() {
    Map<RoadObject> map = Map<RoadObject>(20,50);
//

    test_initials(map);
    test_road(map);


    list<int> orderProducts = {1, 3, 5, 8};
    map.processOrder(13, 17, 0, orderProducts);

    int i =0 ;

//    using my_traits_t2 = restinio::traits_t<
//            restinio::asio_timer_manager_t,
//            restinio::single_threaded_ostream_logger_t,
//            router_t >;

    restinio::run(
            restinio::on_this_thread<>()
                    .address("localhost")
                    .port(8080)
                    .request_handler([map, i ](auto req) mutable{
                        map.nextTick(i++);
                        if (i % 10 == 0){
                            list<int> orderProducts = {1, 3, 5, 8};
//                            map.processOrder(13, 17, i+1, orderProducts);
                        }

                        string body = map.serialize().dump();
//                        std::cout <<"handler :"<< body << endl;
                        return req->create_response()
                                .set_body(std::move( body ))
                                .append_header( restinio::http_field::content_type, "application/json; charset=utf-8")
                                .append_header("Access-Control-Allow-Origin", "*")
                                .done();
                    })
    );
    return 0;
}
