#pragma once

extern bool featurePedAgainstWeapons;
extern bool featureAgainstMeleeWeapons;
extern bool featurePedAgainst;
extern bool featureDriverAgainst;
extern bool featurePoliceAgainst;

// Chance Of Police Calling
const std::vector<std::string> WEAPONS_CHANCEPOLICECALLING_CAPTIONS{ "Zero", "Tiny", "Some", "50/50", "Good", "Great" };
const int WEAPONS_CHANCEPOLICECALLING_VALUES[] = { 0, 2, 10, 30, 50, 70 };
extern int ChancePoliceCallingIndex;
extern bool ChancePoliceCallingChanged;

// Chance Of Attacking You
extern int ChanceAttackingYouIndex;
extern bool ChanceAttackingYouChanged;

void peds_dont_like_weapons();