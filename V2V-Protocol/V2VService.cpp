#include "V2VService.hpp"
#include <cmath>
#include <queue>

std::shared_ptr<cluon::OD4Session> od4;
std::queue <float> delayQueue;

int main(int argc, char **argv) {
    int retCode{0};
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("ip") || 0 == commandlineArguments.count("id") ||
        0 == commandlineArguments.count("left") || 0 == commandlineArguments.count("right") ||
        0 == commandlineArguments.count("speed") || 0 == commandlineArguments.count("queue")) {
        std::cerr << argv[0] << " sends and receives follower-/leader-status in accordance to the DIT168 V2V protocol." << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --cid=<OD4Session components> --freq=<frequency> --ip=<onV2VNetwork> --id=<DIT168Group> --left=<Offset number for left turn> --right<Offset number for right turn> --speed=<Offset in speed for V2V --queue=<Number of max values in queue> " << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=191 --freq=8 --ip=172.20.10.10 --id=8 --left= 0.20 --right=0.17 --speed=0.1 --queue=17" << std::endl;
        retCode = 1;
    } else {
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);

        const std::string IP = commandlineArguments["ip"];
        const std::string ID = commandlineArguments["id"];

        const float leftOffset = std::stof(commandlineArguments["left"]);
        const float rightOffset = std::stof(commandlineArguments["right"]);
        const float speedOffset = std::stof(commandlineArguments["speed"]);
        const uint16_t queue = (uint16_t)std::stoi(commandlineArguments["queue"]);
    
        
    std::shared_ptr<V2VService> v2vService = std::make_shared<V2VService>(IP, ID, leftOffset, rightOffset, speedOffset, queue);

    float pedalPos = 0, steeringAngle = 0;

    od4 = std::make_shared<cluon::OD4Session>(CID,[&pedalPos, &steeringAngle](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            steeringAngle = receivedMsg.steeringAngle();
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            pedalPos = receivedMsg.percent();
        }
    });

    while (1) {
        AnnouncePresence msgAnnounce;
        FollowRequest msgFollowRequest;
        FollowResponse msgFollowResponse;
        StopFollow msgStopFollow;
        LeaderStatus msgLeaderStatus;
        FollowerStatus msgFollowerStatus;

        int choice;
        std::string groupId;
        std::cout << "Which message would you like to send?" << std::endl;
        std::cout << "(1) AnnouncePresence" << std::endl;
        std::cout << "(2) FollowRequest" << std::endl;
        std::cout << "(3) FollowResponse" << std::endl;
        std::cout << "(4) StopFollow" << std::endl;
        std::cout << "(5) LeaderStatus" << std::endl;
        std::cout << "(6) FollowerStatus" << std::endl;
        std::cout << "(#) Nothing, just quit." << std::endl;
        std::cout << ">> ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                v2vService->announcePresence();
                msgAnnounce.vehicleIp(IP);
                msgAnnounce.groupId(ID);
                od4->send(msgAnnounce); 
                break;
            }
            case 2: {
                std::cout << "Which group do you want to follow?" << std::endl;
                std::cin >> groupId;
                if (v2vService->presentCars.find(groupId) != v2vService->presentCars.end()){
                    v2vService->followRequest(v2vService->presentCars[groupId]);}
                else {std::cout << "Sorry, unable to locate that groups vehicle!" << std::endl;}
                od4->send(msgFollowRequest);
                break;
            }
            case 3: {
                v2vService->followResponse(); 
                od4->send(msgFollowResponse);       
                break;
            }
            case 4: {
                std::cout << "Which group do you want to stop follow?" << std::endl;
                std::cin >> groupId;
                if (v2vService->presentCars.find(groupId) != v2vService->presentCars.end()){
                    v2vService->stopFollow(v2vService->presentCars[groupId]);}
                else {std::cout << "Sorry, unable to locate that groups vehicle!" << std::endl;}
                od4->send(msgStopFollow);
                break;
            }    
            case 5: {
                auto sendLoop{[&msgLeaderStatus, &v2vService, &pedalPos, &steeringAngle]() -> bool {

                float roundAngle = (steeringAngle - 0.13f);
                roundAngle = roundf(roundAngle * 100) / 100.0;
                v2vService->leaderStatus(pedalPos, roundAngle, 100);

                msgLeaderStatus.speed(pedalPos);
                msgLeaderStatus.steeringAngle(roundAngle);
                msgLeaderStatus.distanceTraveled(100);
                od4->send(msgLeaderStatus);

                std::cout << "Sending leaderStatus with pedalposition: " << pedalPos << " and steeringangle: " << (roundAngle) << std::endl;

                return true;
                }};

                od4->timeTrigger(FREQ, sendLoop);
                break;
            }
            case 6: {
                v2vService->followerStatus(); 
                od4->send(msgFollowerStatus);
                break;
            }
            default: {
                exit(0);
            	}    
        	}
    	}
	}
}

/**
 * Implementation of the V2VService class as declared in V2VService.hpp
 */
V2VService::V2VService(std::string ip, std::string id, float left, float right, float speed, uint16_t queue) {
    v2vIP = ip;
	v2vID = id;
	LEFT = left;
	RIGHT = right;
	SPEED = speed;
	QUEUE = queue;

	//const float v2vOffSet{0.13};
    /*
     * The broadcast field contains a reference to the broadcast channel which is an OD4Session. This is where
     * AnnouncePresence messages will be received.
     */
    broadcast =
        std::make_shared<cluon::OD4Session>(BROADCAST_CHANNEL,
          [this](cluon::data::Envelope &&envelope) noexcept {
              std::cout << "[OD4] ";
              switch (envelope.dataType()) {
                  case ANNOUNCE_PRESENCE: {
                      AnnouncePresence ap = cluon::extractMessage<AnnouncePresence>(std::move(envelope));
                      std::cout << "received 'AnnouncePresence' from '"
                                << ap.vehicleIp() << "', GroupID '"
                                << ap.groupId() << "'!" << std::endl;

                      presentCars[ap.groupId()] = ap.vehicleIp();
                      od4->send(ap);
                      break;
                  }
                  default: std::cout << "¯\\_(ツ)_/¯" << std::endl;
              }
          });

    /*
     * Each car declares an incoming UDPReceiver for messages directed at them specifically. This is where messages
     * such as FollowRequest, FollowResponse, StopFollow, etc. are received.
     */
    incoming =
        std::make_shared<cluon::UDPReceiver>("0.0.0.0", DEFAULT_PORT,
           [this](std::string &&data, std::string &&sender, std::chrono::system_clock::time_point &&ts) noexcept {
               std::cout << "[UDP] ";
               std::pair<int16_t, std::string> msg = extract(data);

               switch (msg.first) {
                   case FOLLOW_REQUEST: {
                       FollowRequest followRequest = decode<FollowRequest>(msg.second);
                       std::cout << "received '" << followRequest.LongName()
                                 << "' from '" << sender << "'!" << std::endl;

                       // After receiving a FollowRequest, check first if there is currently no car already following.
                       if (followerIp.empty()) {
                          unsigned long len = sender.find(':');    // If no, add the requester to known follower slot
                          followerIp = sender.substr(0, len);      // and establish a sending channel.
                          toFollower = std::make_shared<cluon::UDPSender>(followerIp, DEFAULT_PORT);
                          followResponse();

                       }
                       od4->send(followRequest);
                       break;
                   }
                   case FOLLOW_RESPONSE: {
                       FollowResponse followResponse = decode<FollowResponse>(msg.second);
                       std::cout << "received '" << followResponse.LongName()
                                 << "' from '" << sender << "'!" << std::endl;
                       od4->send(followResponse);
                       break;
                   }
                   case STOP_FOLLOW: {
                       StopFollow stopFollow = decode<StopFollow>(msg.second);
                       std::cout << "received '" << stopFollow.LongName()
                                 << "' from '" << sender << "'!" << std::endl;

                       // Clear either follower or leader slot, depending on current role.
                       unsigned long len = sender.find(':');
                       if (sender.substr(0, len) == followerIp) {
                           followerIp = "";
                           toFollower.reset();
                       }
                       else if (sender.substr(0, len) == leaderIp) {
                           leaderIp = "";
                           toLeader.reset();
                       }
                       od4->send(stopFollow);
                       break;
                   }
                   case FOLLOWER_STATUS: {
                       FollowerStatus followerStatus = decode<FollowerStatus>(msg.second);
                       std::cout << "received '" << followerStatus.LongName()
                                 << "' from '" << sender << "'!" << std::endl;

                       /* TODO: implement lead logic (if applicable) */
                       od4 -> send(followerStatus);
                       break;
                   }
                   case LEADER_STATUS: {
                       LeaderStatus leaderStatus = decode<LeaderStatus>(msg.second);
                       std::cout << leaderStatus.timestamp() << "  ---  Received speed: '" << leaderStatus.speed() << " and angle: " << leaderStatus.steeringAngle()
                                 << "' from '" << sender << "'!    -    " << leaderStatus.distanceTraveled() << std::endl;

                       	/* TODO: completely implement follow logic */
                       	od4->send(leaderStatus);

                      	opendlv::proxy::GroundSteeringReading msgSteering;
    					opendlv::proxy::PedalPositionReading msgPedal;

    					msgPedal.percent(leaderStatus.speed() - SPEED);
    					od4->send(msgPedal);

    					float calibratedAngle;

    					if(leaderStatus.steeringAngle() == 0){
    						calibratedAngle = 0.13f;
    					}
    					else if(leaderStatus.steeringAngle() > 0){
    						calibratedAngle = 0.13f + LEFT + leaderStatus.steeringAngle();
    					}
    					else{
    						calibratedAngle = 0.13f + RIGHT + leaderStatus.steeringAngle();
    					}


    					if(leaderStatus.speed() != 0 && delayQueue.size() < QUEUE){
    						delayQueue.push(calibratedAngle);
    					}
    					else if(leaderStatus.speed() != 0 && delayQueue.size() >= QUEUE){
    						delayQueue.push(calibratedAngle);

    						msgSteering.steeringAngle(delayQueue.front());
    						od4->send(msgSteering);
    						delayQueue.pop();
    					}
                       break;
                   }
                   default: std::cout << "¯\\_(ツ)_/¯" << std::endl;
               }
           });
		}

/**
 * This function sends an AnnouncePresence (id = 1001) message on the broadcast channel. It will contain information
 * about the sending vehicle, including: IP, port and the group identifier.
 */
void V2VService::announcePresence() {
    if (!followerIp.empty()) return;
    AnnouncePresence announcePresence;
    announcePresence.vehicleIp(v2vIP);
    announcePresence.groupId(v2vID);
    broadcast->send(announcePresence);
}

/**
 * This function sends a FollowRequest (id = 1002) message to the IP address specified by the parameter vehicleIp. And
 * sets the current leaderIp field of the sending vehicle to that of the target of the request.
 *
 * @param vehicleIp - IP of the target for the FollowRequest
 */
void V2VService::followRequest(std::string vehicleIp) {
    if (!leaderIp.empty()) return;
    leaderIp = vehicleIp;
    toLeader = std::make_shared<cluon::UDPSender>(leaderIp, DEFAULT_PORT);
    FollowRequest followRequest;
    toLeader->send(encode(followRequest));
}

/**
 * This function send a FollowResponse (id = 1003) message and is sent in response to a FollowRequest (id = 1002).
 * This message will contain the NTP server IP for time synchronization between the target and the sender.
 */
void V2VService::followResponse() {
    if (followerIp.empty()) return;
    FollowResponse followResponse;
    toFollower->send(encode(followResponse));
}

/**
 * This function sends a StopFollow (id = 1004) request on the ip address of the parameter vehicleIp. If the IP address is neither
 * that of the follower nor the leader, this function ends without sending the request message.
 *
 * @param vehicleIp - IP of the target for the request
 */
void V2VService::stopFollow(std::string vehicleIp) {
    StopFollow stopFollow;
    if (vehicleIp == leaderIp) {
        toLeader->send(encode(stopFollow));
        leaderIp = "";
        toLeader.reset();
    }
    if (vehicleIp == followerIp) {
        toFollower->send(encode(stopFollow));
        followerIp = "";
        toFollower.reset();
    }
}

/**
 * This function sends a FollowerStatus (id = 3001) message on the leader channel.
 *
 * @param speed - current velocity
 * @param steeringAngle - current steering angle
 * @param distanceFront - distance to nearest object in front of the car sending the status message
 * @param distanceTraveled - distance traveled since last reading
 */
void V2VService::followerStatus() {
    if (leaderIp.empty()) return;
    FollowerStatus followerStatus;
    toLeader->send(encode(followerStatus));
}

/**
 * This function sends a LeaderStatus (id = 2001) message on the follower channel.
 *
 * @param speed - current velocity
 * @param steeringAngle - current steering angle
 * @param distanceTraveled - distance traveled since last reading
 */
void V2VService::leaderStatus(float speed, float steeringAngle, uint8_t distanceTraveled) {
    if (followerIp.empty()) return;
    LeaderStatus leaderStatus;
    leaderStatus.timestamp(getTime());
    leaderStatus.speed(speed);
    leaderStatus.steeringAngle(steeringAngle);
    leaderStatus.distanceTraveled(distanceTraveled);
    toFollower->send(encode(leaderStatus));
}

/**
 * Gets the current time.
 *
 * @return current time in milliseconds
 */
uint32_t V2VService::getTime() {
    timeval now;
    gettimeofday(&now, nullptr);
    return (uint32_t ) now.tv_usec / 1000;
}

/**
 * The extraction function is used to extract the message ID and message data into a pair.
 *
 * @param data - message data to extract header and data from
 * @return pair consisting of the message ID (extracted from the header) and the message data
 */
std::pair<int16_t, std::string> V2VService::extract(std::string data) {
    if (data.length() < 10) return std::pair<int16_t, std::string>(-1, "");
    int id, len;
    std::stringstream ssId(data.substr(0, 4));
    std::stringstream ssLen(data.substr(4, 10));
    ssId >> std::hex >> id;
    ssLen >> std::hex >> len;
    return std::pair<int16_t, std::string> (
            data.length() -10 == len ? id : -1,
            data.substr(10, data.length() -10)
    );
};

/**
 * Generic encode function used to encode a message before it is sent.
 *
 * @tparam T - generic message type
 * @param msg - message to encode
 * @return encoded message
 */
template <class T>
std::string V2VService::encode(T msg) {
    cluon::ToProtoVisitor v;
    msg.accept(v);
    std::stringstream buff;
    buff << std::hex << std::setfill('0')
         << std::setw(4) << msg.ID()
         << std::setw(6) << v.encodedData().length()
         << v.encodedData();
    return buff.str();
}

/**
 * Generic decode function used to decode an incoming message.
 *
 * @tparam T - generic message type
 * @param data - encoded message data
 * @return decoded message
 */
template <class T>
T V2VService::decode(std::string data) {
    std::stringstream buff(data);
    cluon::FromProtoVisitor v;
    v.decodeFrom(buff);
    T tmp = T();
    tmp.accept(v);
    return tmp;
}