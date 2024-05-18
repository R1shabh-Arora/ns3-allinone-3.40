#ifndef SHARED_VARS_H
#define SHARED_VARS_H

// Add a doxygen group for this module.
// If you have more than one file, this should be in only one of them.
/**
 * \defgroup shared_vars Description of the shared_vars
 */

#include "ns3/object.h"

namespace ns3
{

// Each class should be documented using Doxygen,
// and have an \ingroup shared_vars directive

// define a strategy enum here to distinguish between strategies:
enum AttackStratSelect
{
    NO_A_OPERATION,
    PACKET_DROP_PERC,
    PACKET_DROP_CONNECTION,
    PACKET_DROP_NEIGHBOURS,
    PACKET_DROP_IN_TIME,
    PACKET_DROP_SELECT
};

// define a strategy enum here to distinguish between strategies:
enum DefenseStratSelect
{
    NO_D_OPERATION,
    MONITOR_ALWAYS,
    MONITOR_WHEN_VELOCITY,
    TRAINING,
    INFERENCE
};

// Define a new class here:
class TargetNodes : public Object
{
  public:
    std::vector<uint32_t> node;
    std::vector<float> connection_strength;
    std::vector<float> d_connection_strength;
};

class DetectedPacketClass : public Object
{
  public:
    std::vector<uint16_t> ids;
    std::vector<uint8_t> ttl;
};

class DetectionResultsClass : public Object
{
  public:
    uint32_t fp;
    uint32_t fn;
    uint32_t tp;
    uint32_t tn;
};

class PacketsExpected : public Object
{
  public:
    std::vector<uint16_t> ids;
    std::vector<uint32_t> node;
    std::vector<uint8_t> ttl;
};

class ForwardTableEntry : public Object
{
  public:
    uint32_t node;
    uint32_t forwardCount;
    uint32_t noForwardCount;
};

class MassTableEntry : public Object
{
  public:
    uint16_t node_recommended;
    uint16_t node_subject;
    float mTrust;
    float mDistrust;
    float mUncertain;
};

class TrustValueEntry : public Object
{
  public:
    uint32_t node;
    float alpha;
    float beta;
};

class GymActionVariables : public Object
{
  public:
    float reject_node;
};

class GymRewardVariables : public Object
{
  public:
    float value;
    float gameover;
};

class GymStateVariables : public Object
{
  public:
    std::vector<float> context;
    std::vector<float> current_speed;
    std::vector<float> d_distance;
    std::vector<float> distance;
};

class GymVariables : public Object
{
  public:
    uint32_t reward_node;
    uint32_t next_node;
    Ptr<GymStateVariables> state;
    Ptr<GymRewardVariables> reward;
    Ptr<GymActionVariables> action;
};

class DroppedStats : public Object
{
  public:
    std::vector<uint32_t> drop_count;
};
}

#endif /* SHARED_VARS_H */
