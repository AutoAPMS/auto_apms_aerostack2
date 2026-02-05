#include "as2_msgs/srv/set_control_mode.hpp"

#include "auto_apms_behavior_tree_core/node.hpp"

#define INPUT_KEY_DRONE "drone"
#define INPUT_KEY_YAW_MODE "yaw_mode"
#define INPUT_KEY_CONTROL_MODE "control_mode"
#define INPUT_KEY_REFERENCE_FRAME "reference_frame"

namespace auto_apms_aerostack2
{

class SetControlMode : public auto_apms_behavior_tree::core::RosServiceNode<as2_msgs::srv::SetControlMode>
{
public:
  SetControlMode(const std::string & instance_name, const Config & config, const Context & context)
  : RosServiceNode(instance_name, config, context)
  {
  }

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>(INPUT_KEY_DRONE, "drone0", "Name of the drone to set the control mode for."),
      BT::InputPort<uint8_t>(INPUT_KEY_YAW_MODE, 0, "Yaw control mode (0: NONE - 1: YAW_ANGLE - 2: YAW_SPEED)."),
      BT::InputPort<uint8_t>(
        INPUT_KEY_CONTROL_MODE, 0,
        "Control mode (0: UNSET - 1: HOVER - 2: POSITION - 3: SPEED - 4: SPEED_IN_A_PLANE - 5: ATTITUDE - 6: ACRO - 7: "
        "TRAJECTORY)."),
      BT::InputPort<uint8_t>(
        INPUT_KEY_REFERENCE_FRAME, 0,
        "Reference frame (0: UNDEFINED_FRAME - 1: LOCAL_ENU_FRAME - 2: BODY_FLU_FRAME - 3: GLOBAL_LAT_LONG_ASML)."),
    };
  }

  bool setRequest(Request::SharedPtr & request) override final
  {
    request->control_mode.yaw_mode = getInput<uint8_t>(INPUT_KEY_YAW_MODE).value();
    if (request->control_mode.yaw_mode > 2) {
      RCLCPP_ERROR(
        logger_, "%s - Yaw control mode %i is undefined", context_.getFullyQualifiedTreeNodeName(this).c_str(),
        request->control_mode.yaw_mode);
      return false;
    }
    request->control_mode.control_mode = getInput<uint8_t>(INPUT_KEY_CONTROL_MODE).value();
    if (request->control_mode.control_mode > 7) {
      RCLCPP_ERROR(
        logger_, "%s - Control mode %i is undefined", context_.getFullyQualifiedTreeNodeName(this).c_str(),
        request->control_mode.control_mode);
      return false;
    }
    request->control_mode.reference_frame = getInput<uint8_t>(INPUT_KEY_REFERENCE_FRAME).value();
    if (request->control_mode.reference_frame > 3) {
      RCLCPP_ERROR(
        logger_, "%s - Reference frame %i is undefined", context_.getFullyQualifiedTreeNodeName(this).c_str(),
        request->control_mode.reference_frame);
      return false;
    }
    return true;
  }

  BT::NodeStatus onResponseReceived(const Response::SharedPtr & response) override final
  {
    return response->success ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
  }
};

}  // namespace auto_apms_aerostack2

AUTO_APMS_BEHAVIOR_TREE_REGISTER_NODE(auto_apms_aerostack2::SetControlMode)
