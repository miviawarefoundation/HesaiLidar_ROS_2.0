#include "hesai_ros_driver_component.hpp"

#include "manager/source_driver_ros2.hpp"

#include <rclcpp_components/register_node_macro.hpp>
#include <yaml-cpp/yaml.h>

#include <cstddef>
#include <stdexcept>
#include <string>

namespace hesai_ros_driver
{

HesaiRosDriverNode::HesaiRosDriverNode(const rclcpp::NodeOptions & options)
: Node("hesai_ros_driver_node", options)
{
  const auto config_path = declare_parameter<std::string>("config_path", "");
  const auto lidar_index = declare_parameter<int>("lidar_index", -1);

  if (config_path.empty()) {
    throw std::invalid_argument("config_path must identify a Hesai YAML configuration");
  }

  const YAML::Node config = YAML::LoadFile(config_path);
  const YAML::Node lidars = config["lidar"];
  if (!lidars || !lidars.IsSequence()) {
    throw std::invalid_argument("Hesai configuration must contain a lidar sequence");
  }
  if (lidar_index < 0 || static_cast<std::size_t>(lidar_index) >= lidars.size()) {
    throw std::out_of_range("lidar_index is outside the configured lidar sequence");
  }

  source_driver_ = std::make_shared<SourceDriver>(SourceType::DATA_FROM_LIDAR);
  source_driver_->Init(lidars[static_cast<std::size_t>(lidar_index)], *this);
  source_driver_->Start();

  RCLCPP_INFO(get_logger(), "Started Hesai source at lidar index %ld", static_cast<long>(lidar_index));
}

}  // namespace hesai_ros_driver

RCLCPP_COMPONENTS_REGISTER_NODE(hesai_ros_driver::HesaiRosDriverNode)
