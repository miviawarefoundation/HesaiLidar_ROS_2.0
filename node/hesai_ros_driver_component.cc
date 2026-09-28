#include "manager/node_manager.h"

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <yaml-cpp/yaml.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace hesai_ros_driver
{

class HesaiRosDriverNode final : public rclcpp::Node
{
public:
  explicit HesaiRosDriverNode(const rclcpp::NodeOptions & options)
  : Node("hesai_ros_driver_node", options)
  {
    const std::string default_path = std::string(PROJECT_PATH) + "/config/config.yaml";
    const auto config_path = declare_parameter<std::string>("config_path", default_path);
    try {
      manager_ = std::make_unique<NodeManager>();
      manager_->Init(YAML::LoadFile(config_path), this);
      manager_->Start();
      RCLCPP_INFO(get_logger(), "Hesai driver started from %s", config_path.c_str());
    } catch (const std::exception & error) {
      manager_.reset();
      throw std::runtime_error(
              std::string("cannot initialize Hesai driver from ") + config_path + ": " +
              error.what());
    }
  }

  ~HesaiRosDriverNode() override
  {
    if (manager_) {
      manager_->Stop();
    }
  }

private:
  std::unique_ptr<NodeManager> manager_;
};

}  // namespace hesai_ros_driver

RCLCPP_COMPONENTS_REGISTER_NODE(hesai_ros_driver::HesaiRosDriverNode)
