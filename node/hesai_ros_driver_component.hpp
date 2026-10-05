#pragma once

#include <rclcpp/rclcpp.hpp>

#include <memory>

class SourceDriver;

namespace hesai_ros_driver
{

class HesaiRosDriverNode : public rclcpp::Node
{
public:
  explicit HesaiRosDriverNode(const rclcpp::NodeOptions & options);
  ~HesaiRosDriverNode() override = default;

private:
  std::shared_ptr<SourceDriver> source_driver_;
};

}  // namespace hesai_ros_driver
