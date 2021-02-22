// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319


#include "DiscordObject.h"
#include "../discord-files/discord.h"
#include "UObject/Class.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscord, Log, All)

#define LogDisplay(Param1)		UE_LOG(LogDiscord, Display, TEXT("%s"), *FString(Param1))
#define LogError(Param1)		UE_LOG(LogDiscord, Error, TEXT("%s"), *FString(Param1))

discord::Core* core{};
discord::Activity activity{};

UDiscordObject* UDiscordObject::DiscordObjectInstance = nullptr;


UDiscordObject::UDiscordObject()
{
	bCanTick = bTimerStarted = false;
}

UDiscordObject* UDiscordObject::GetOrCreateDiscordObject(FString InClientID, const bool bRequireDiscordRunning /*= false*/, const bool bStartElapsedTimer /*= true*/)
{
	if (DiscordObjectInstance == nullptr)
	{
		CreateDiscordObject(InClientID, bRequireDiscordRunning, bStartElapsedTimer);
	}
	
	return DiscordObjectInstance;
}

void UDiscordObject::CreateDiscordObject(FString InClientID, const bool bRequireDiscordRunning /*= false*/, const bool bStartElapsedTimer /*= true*/)
{
	if (DiscordObjectInstance == nullptr)
	{
		DiscordObjectInstance = NewObject<UDiscordObject>();
		DiscordObjectInstance->AddToRoot();
		DiscordObjectInstance->Internal_CreateDiscordObject(InClientID, bRequireDiscordRunning, bStartElapsedTimer);
	}
}

UDiscordObject* UDiscordObject::GetDiscordObject()
{
	return DiscordObjectInstance;
}

void UDiscordObject::DestroyDiscordObject()
{
	if (DiscordObjectInstance)
	{
		DiscordObjectInstance->SetState("");
		DiscordObjectInstance->SetDetails("");
		DiscordObjectInstance->StopDiscordTimer();
		DiscordObjectInstance->bCanTick = false;
		delete core;
		core = nullptr;
		DiscordObjectInstance->RemoveFromRoot();
		DiscordObjectInstance->ConditionalBeginDestroy();
		DiscordObjectInstance = nullptr;
		LogDisplay("Discord object destroyed.");
	}
}

const FString UDiscordObject::GetDiscordResultString(EDiscordReturnResult InDiscordResult)
{
	return UEnum::GetDisplayValueAsText(InDiscordResult).ToString();
}

void UDiscordObject::SetState(FString InNewState)
{
	activity.SetState(TCHAR_TO_UTF8(*InNewState));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnStateSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("State Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::SetDetails(FString InNewDetails)
{
	activity.SetDetails(TCHAR_TO_UTF8(*InNewDetails));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnDetailsSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("Details Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::SetLargeImage(const FString InKeyName)
{
	activity.GetAssets().SetLargeImage(TCHAR_TO_UTF8(*InKeyName));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnLargeImageSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("Large Image Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::SetSmallImage(const FString InKeyName)
{
	activity.GetAssets().SetSmallImage(TCHAR_TO_UTF8(*InKeyName));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnSmallImageSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("Small Image Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::SetLargeImageText(FString InNewDetails)
{
	activity.GetAssets().SetLargeText(TCHAR_TO_UTF8(*InNewDetails));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnLargeTextSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Image large set text. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::SetSmallImageText(FString InNewDetails)
{
	activity.GetAssets().SetSmallText(TCHAR_TO_UTF8(*InNewDetails));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnSmallTextSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Image small set text. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::SetPartyId(FString InNewPartyId)
{
	activity.GetParty().SetId(TCHAR_TO_UTF8(*InNewPartyId));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnPartyIdSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Party id set. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::SetPartySize(int32 InNewPartySize)
{
	activity.GetParty().GetSize().SetCurrentSize(InNewPartySize);
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnPartySizeSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Party size set. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::SetPartyMax(int32 InNewPartyMax)
{
	activity.GetParty().GetSize().SetMaxSize(InNewPartyMax);
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnPartyMaxSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Party max set. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::SetJoinSecret(FString InNewJoinSecret)
{
	activity.GetSecrets().SetJoin(TCHAR_TO_UTF8(*InNewJoinSecret));
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
			{
				uint8 ResultByte = (uint8)result;
				DiscordObjectInstance->OnJoinSecretSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
				LogDisplay(FString::Printf(TEXT("Join secret set. Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
			});
	}
}

void UDiscordObject::StartDiscordTimer()
{
	activity.GetTimestamps().SetStart(FDateTime::UtcNow().ToUnixTimestamp());
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnTimerStart.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("Timer Start Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::StopDiscordTimer()
{
	DiscordObjectInstance->bTimerStarted = false;
	activity.GetTimestamps().SetEnd(FDateTime::UtcNow().ToUnixTimestamp());
	if (core)
	{
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
		{
			uint8 ResultByte = (uint8)result;
			DiscordObjectInstance->OnTimerEnd.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
			LogDisplay(FString::Printf(TEXT("Timer End Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
		});
	}
}

void UDiscordObject::Internal_CreateDiscordObject(const FString& InClientID, const bool bRequireDiscordRunning, const bool bStartElapsedTimer)
{	
#if WITH_EDITOR
	discord::Result result = discord::Core::Create(FCString::Atoi64(*InClientID), DiscordCreateFlags_NoRequireDiscord, &core);
#else
	discord::Result result = discord::Core::Create(FCString::Atoi64(*InClientID), bRequireDiscordRunning ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &core);
#endif
	if (result == discord::Result::Ok)
	{
		DiscordObjectInstance->bCanTick = true;
		LogDisplay("Discord object created.");

		if (bStartElapsedTimer)
		{
			DiscordObjectInstance->StartDiscordTimer();
		}
	}
}

void UDiscordObject::Tick(float DeltaTime)
{
	if (bCanTick)
	{
		::core->RunCallbacks();
	}
}


#undef LogDisplay
#undef LogError
