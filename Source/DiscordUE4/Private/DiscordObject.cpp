// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319


#include "DiscordObject.h"
#include "../discord-files/discord.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscord, Log, All)

#define LogDisplay(Param1)		UE_LOG(LogDiscord, Display, TEXT("%s"), *FString(Param1))

discord::Core* core{};
discord::Activity activity{};

UDiscordObject* UDiscordObject::DiscordObjectInstance = nullptr;

void UDiscordObject::BeginDestroy()
{
	Super::BeginDestroy();
	if (DiscordObjectInstance)
	{
		DiscordObjectInstance->ConditionalBeginDestroy();
		DiscordObjectInstance = nullptr;
		LogDisplay("Discord object destroyed.");
	}
}

UDiscordObject* UDiscordObject::GetOrCreateDiscordObject(FString InClientID, const bool bRequireDiscordRunning /*= false*/)
{
	if (DiscordObjectInstance == nullptr)
	{
		DiscordObjectInstance = NewObject<UDiscordObject>();
		DiscordObjectInstance->Internal_CreateDiscordObject(InClientID, bRequireDiscordRunning);
	}
	
	return DiscordObjectInstance;
}

void UDiscordObject::SetState(FString InNewState)
{
	activity.SetState(TCHAR_TO_UTF8(*InNewState));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnStateSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
	});
}

void UDiscordObject::SetDetails(FString InNewDetails)
{
	activity.SetDetails(TCHAR_TO_UTF8(*InNewDetails));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnDetailsSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
	});
}

void UDiscordObject::Internal_CreateDiscordObject(const FString& InClientID, const bool bRequireDiscordRunning)
{	
	discord::Result result = discord::Core::Create(FCString::Atoi64(*InClientID), DiscordCreateFlags_Default, &core);
	LogDisplay("Discord object created.");
}

void UDiscordObject::Tick(float DeltaTime)
{
	::core->RunCallbacks();
}

#undef LogDisplay
