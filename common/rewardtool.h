#ifndef REWARD_TOOL_H
#define REWARD_TOOL_H

void RewardHandle();
void BlockRewardHelp();
void BlockReward(int h);

void RewardStatisticsHelp();
void RewardStatistics(int iyears, int isum);

void RewardExampleHelp();
void RewardExample(const std::string & filename);
#endif