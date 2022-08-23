#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "rt2_ros2/srv/random_position.hpp"
#include <iostream>

using namespace std;
using Random_Position = rt2_ros2::srv::RandomPosition;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace rt2_ros2{

    class RandPos : public rclcpp :: Node{
        public:
            RandPos(const rclcpp::NodeOptions & options) : Node("pos", options){
                // position server initialization
                serv = this->create_service<Random_Position>("/position_server", std::bind(&RandPos::handle, this, _1, _2, _3));
            }

        private:
            // function to return a random value in an interval
            double randMToN(double M, double N){
                return M + (rand() / ( RAND_MAX / (N-M) ) ) ; 
                }
            // function that handles the service when the client calls
            void handle(
                const std::shared_ptr<rmw_request_id_t> request_header,
                const std::shared_ptr<Random_Position::Request> req,
                const std::shared_ptr<Random_Position::Response> res
            )
            {
                (void)request_header;
                    // response definition as per RandomPosition.srv
                    res->x = randMToN(req->x_min, req->x_max);
                    res->y = randMToN(req->y_min, req->y_max);
                    res->theta = randMToN(-3.14, 3.14);
            }

            rclcpp::Service<Random_Position>::SharedPtr serv;
    };
}
RCLCPP_COMPONENTS_REGISTER_NODE(rt2_ros2::RandPos)


