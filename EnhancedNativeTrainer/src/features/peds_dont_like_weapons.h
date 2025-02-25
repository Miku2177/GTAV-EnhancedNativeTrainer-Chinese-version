#pragma once

extern bool featurePedAgainstWeapons;
extern bool featureAgainstMeleeWeapons;
extern bool featurePedAgainst;
extern bool featureDriverAgainst;
extern bool featurePoliceAgainst;

// 报警呼叫警察的几率
const std::vector<std::string> WEAPONS_CHANCEPOLICECALLING_CAPTIONS{ "零", "1 成", "3 成", "5 成", "7 成", "9 成" };
const int WEAPONS_CHANCEPOLICECALLING_VALUES[] = { 0, 10, 30, 50, 70, 90 };
extern int ChancePoliceCallingIndex;
extern bool ChancePoliceCallingChanged;

// 攻击你的几率
extern int ChanceAttackingYouIndex;
extern bool ChanceAttackingYouChanged;

void peds_dont_like_weapons();