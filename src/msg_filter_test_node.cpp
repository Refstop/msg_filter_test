#include <iostream>
#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>

using namespace std;
using namespace geometry_msgs;
using namespace message_filters;

void callback(const PoseStamped::ConstPtr& p, const PoseWithCovarianceStamped::ConstPtr& pwc) {
  cout << "pose: " << pwc->pose.pose.position.x << ", " << pwc->pose.pose.position.y << endl;
  cout << "result: " << p->pose.position.x << ", " << p->pose.position.y << endl;

}

int main(int argc, char** argv) {
  ros::init(argc, argv, "msg_filter_test");

  ros::NodeHandle nh;
  message_filters::Subscriber<PoseStamped> pose_sub(nh, "pose_s", 1);
  message_filters::Subscriber<PoseWithCovarianceStamped> result_sub(nh, "result_pose", 1);
  // TimeSynchronizer<PoseStamped, PoseWithCovarianceStamped> sync(pose_sub, result_sub, 10);
  typedef sync_policies::ApproximateTime<PoseStamped, PoseWithCovarianceStamped> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), pose_sub, result_sub);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();
  return 0;
}