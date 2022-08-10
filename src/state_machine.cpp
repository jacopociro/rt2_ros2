#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "rt2_ros2/srv/command.hpp"
#include "rt2_ros2/srv/position.hpp"
#include "rt2_ros2/srv/random_position.hpp"
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

using namespace std;
using Command = rt2_ros2::srv::Command;
using Pos = rt2_ros2::srv::Position;
using Random_Position = rt2_ros2::srv::RandomPosition;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

bool start=false;
bool notdone=true;
namespace rt2_ros2 {

    class GoalClient : public rclcpp::Node{
        public:
            GoalClient() : Node("GoalClient")
                {
                    client = this->create_client<Pos>("/go_to_point");
                    this->req = std::make_shared<Pos::Request>();
                    this->res = std::make_shared<Pos::Response>();
                }
            void GoToPoint_server(){
                auto result = client->async_send_request(req);
                if(rclcpp::spin_until_future_complete(this->get_node_base_interface(), result)!=rclcpp::FutureReturnCode::SUCCESS){
                    RCLCPP_ERROR(this->get_logger(), "ERROR: service call");
                }
                this->res = result.get();
            }
            std::shared_ptr<Pos::Request> req;
            std::shared_ptr<Pos::Response> res;
        
        private:
            rclcpp::Client<Pos>::SharedPtr client;
    };

    class FSM : public rclcpp::Node{
        public: 
            FSM(const rclcpp::NodeOptions & options) : Node("FSM", options){
                clock = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&FSM::clock_cb, this));
                srv = this->create_service<Command>("/user_interface", std::bind(&FSM::handle, this, _1, _2, _3));

                Pos_Client = this->create_client<Random_Position>("/position_server");
            }

        private:
            void handle(const std::shared_ptr<rmw_request_id_t> header, const std::shared_ptr<Command::Request> request, const std::shared_ptr<Command::Response> response){
                (void) header;
                if(request->command == "stop"){
                    std::cout<<"stop received"<<std::endl;
                    start = false;
                    response->ok = true;
                }
                else 
                    {
                        start = true;
                        response->ok=true;
                    }
            }
            void clock_cb(){
                auto RandPos = std::make_shared<Random_Position::Request>();
                if(notdone == false){
                    notdone = true;
                }
                else if(start == false){

                }
                else if (start){
                    RandPos->x_max = 5.0;
                    RandPos->x_min = -5.0;
                    RandPos->y_max = 5.0;
                    RandPos->y_min = -5.0;

                    using ServiceFuture = rclcpp::Client<Random_Position>::SharedFuture;
                    auto res_rec_cb = [this](ServiceFuture future){
                        auto goal = std::make_shared<GoalClient>();
                        goal->req->x=future.get()->x;
                        goal->req->y=future.get()->y;
                        goal->req->theta=future.get()->theta;
                        std::cout << "\nGoing to: x= " << goal->req->x << " y= " <<goal->req->y << " theta = " <<goal->req->theta << std::endl;
                        goal->GoToPoint_server();
                        std::cout<<"Goal Reached"<<std::endl;

                    };
                    auto result = Pos_Client->async_send_request(RandPos, res_rec_cb);
                    notdone = false;
                }
            }
            rclcpp::Service<Command>::SharedPtr srv;
            rclcpp::TimerBase::SharedPtr clock;
            rclcpp::Client<Random_Position>::SharedPtr Pos_Client;
    };
}

RCLCPP_COMPONENTS_REGISTER_NODE(rt2_ros2::FSM)