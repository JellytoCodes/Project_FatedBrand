// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_AbilityInfo.h"

FFatedBrandAbilityInfo UDataAsset_AbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const
{
	for (const FFatedBrandAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	return FFatedBrandAbilityInfo();
}
