#include "classes/MapPoint.h"
#include "classes/Map.h"
#include "models/product.h"
#include <iostream>
#include <unistd.h>
#include <restinio/all.hpp>

using router_t = restinio::router::express_router_t<>;


int test_initials(Map<RoadObject> & map){
    //test insert
    RoadObject start_road = map.insert(10, 15);
    map.insert(11, 15);
    RoadObject end_road = map.insert(12, 15);

    map.insert(11, 16);
    map.insert(11, 17);
    RoadObject second_start_road = map.insert(11, 17);
    map.insert(12, 17);
    map.insert(13, 17);
    map.insert(12, 16);

    list<int> a  = {1,2,3,4,5};
    list<int> b = {6,7,8,9,10};

    map.addStore(10, 15, "TestStore", a);
    map.addStore(20, 15, "TestStore2", b);

    list<int> orderProducts = {1,  3,  5,  8};
    map.processOrder(13,17, 0, orderProducts);

    map.update(11, 17, second_start_road);
    map.update(10, 15, start_road);
    map.update(12, 15, end_road);
}

int main() {
    Map<RoadObject> map = Map<RoadObject>(100,100);
    test_initials(map);
    int i;
    using my_traits_t2 = restinio::traits_t<
            restinio::asio_timer_manager_t,
            restinio::single_threaded_ostream_logger_t,
            router_t >;

    restinio::run(
            restinio::on_this_thread<>()
                    .address("localhost")
                    .port(8080)
                    .request_handler([map, i ](auto req) mutable{
                        map.nextTick(i++);

                        string body = map.serialize().dump();
                        std::cout <<"handler :"<< body << endl;
                        return req->create_response()
                        .set_body(std::move( body ))
                        .append_header( restinio::http_field::content_type, "application/json; charset=utf-8" )
                        .done();
                    })
            );
    return 0;
}
