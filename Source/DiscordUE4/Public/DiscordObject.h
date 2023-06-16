// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319

#pragma once

#include "Tickable.h"
#include "DiscordObject.generated.h"

UENUM(BlueprintType)
enum class EDiscordReturnResult : uint8
{
	Ok = 0,
	ServiceUnavailable = 1,
	InvalidVersion = 2,
	LockFailed = 3,
	InternalError = 4,
	InvalidPayload = 5,
	InvalidCommand = 6,
	InvalidPermissions = 7,
	NotFetched = 8,
	NotFound = 9,
	Conflict = 10,
	InvalidSecret = 11,
	InvalidJoinSecret = 12,
	NoEligibleActivity = 13,
	InvalidInvite = 14,
	NotAuthenticated = 15,
	InvalidAccessToken = 16,
	ApplicationMismatch = 17,
	InvalidDataUrl = 18,
	InvalidBase64 = 19,
	NotFiltered = 20,
	LobbyFull = 21,
	InvalidLobbySecret = 22,
	InvalidFilename = 23,
	InvalidFileSize = 24,
	InvalidEntitlement = 25,
	NotInstalled = 26,
	NotRunning = 27,
	InsufficientBuffer = 28,
	PurchaseCanceled = 29,
	InvalidGuild = 30,
	InvalidEvent = 31,
	InvalidChannel = 32,
	InvalidOrigin = 33,
	RateLimited = 34,
	OAuth2Error = 35,
	SelectChannelTimeout = 36,
	GetGuildTimeout = 37,
	SelectVoiceForceRequired = 38,
	CaptureShortcutAlreadyListening = 39,
	UnauthorizedForAchievement = 40,
	InvalidGiftCode = 41,
	PurchaseError = 42,
	TransactionAborted = 43,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordResult, EDiscordReturnResult, StateSetResult);

UCLASS(NotBlueprintable, BlueprintType)
class DISCORDUE4_API UDiscordObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	static TStrongObjectPtr<UDiscordObject> DiscordObjectInstance;

	uint8 bCanTick : 1;
	uint8 bTimerStarted : 1;

public:

	UDiscordObject();

	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnStateSet;

	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnDetailsSet;

	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnLargeImageSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnSmallImageSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnPartyIdSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnPartySizeSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnPartyMaxSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnJoinSecretSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnSpectateSecretSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnLargeTextSet;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnSmallTextSet;

	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnTimerStart;

	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordResult OnTimerEnd;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordActivitySpectate, FString, SpectateSecret);
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordActivitySpectate OnActivitySpectate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordActivityJoin, FString, JoinSecret);
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordActivityJoin OnActivityJoin;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiscordActivityJoinRequest, FString, UserId, FString, UserName);
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordActivityJoinRequest OnActivityJoinRequest;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiscordActivityInvite, FString, UserId, FString, UserName);
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordActivityInvite OnActivityInviteToParty;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordActivityInvite OnActivityInviteToSpectate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordUserUpdated, FString, UserId);
	UPROPERTY(BlueprintAssignable, Category = "Discord|Delegates")
	FOnDiscordUserUpdated OnDiscordUserUpdated;

	/**
	* public static UDiscordObject::CreateDiscordObject
	* Creates the static Discord Object Instance.
	* @See Make sure you setup your app as per this documentation https://discord.com/developers/docs/game-sdk/sdk-starter-guide
	* @param InClientID [FString] The client ID of your application after creating it in https://discord.com/developers/
	* @param bRequireDiscordRunning [const bool] If true, the game will close, opens Discord, and will try to launch your game. IMPORTANT NOTE: Only works in packaged game and NOT in Editor, PIE etc.
	* @param bStartElapsedTimer [const bool] If true, rich presence will show elapsed time. You can manually start time stamps by calling Start/Stop Discord Timer.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")	
	static void CreateDiscordObject(FString InClientID, const bool bRequireDiscordRunning = false, const bool bStartElapsedTimer = true);

	/**
	* public static UDiscordObject::GetDiscordObject
	* Returns static DiscordObjectInstance.
	* @return [UDiscordObject*] Discord object instance if valid.
	**/
	UFUNCTION(BlueprintPure, Category = "Discord")	
	static UDiscordObject* GetDiscordObject();

	/**
	* public static UDiscordObject::DestroyDiscordObject
	* Destroys the static Discord Object Instance.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")	
	static void DestroyDiscordObject();

	/**
	* public static UDiscordObject::GetDiscordResultString
	* Returns a string representation of given discord result. Can be used with Discord delegates. @See OnStateSet, OnDetailsSet etc.
	* @param InDiscordResult [EDiscordReturnResult] Result to check for.
	* @return [const FString] Human readable string representation of the given result.
	**/
	UFUNCTION(BlueprintPure, Category = "Discord")	
	static const FString GetDiscordResultString(EDiscordReturnResult InDiscordResult);

	/**
	* public UDiscordObject::SetState
	* Sets a new state that you can see on Discord Rich Presence.
	* @See OnStateSet delegate.
	* @param InNewState [FString] State to set.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetState(FString InNewState);

	/**
	* public UDiscordObject::SetDetails
	* Sets new details that you can see on Discord Rich Presence.
	* @See OnDetailsSet delegate.
	* @param InNewDetails [FString] Details to set.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetDetails(FString InNewDetails);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetLargeImage(const FString InKeyName);
	
	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetSmallImage(const FString InKeyName);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetLargeImageText(const FString InKeyName);
	
	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetSmallImageText(const FString InKeyName);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetPartyId(const FString InNewPartyId);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetPartySize(const int32 InNewPartySize);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetPartyMax(const int32 InNewPartyMax);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetJoinSecret(const FString InNewJoinSecret);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetSpectateSecret(const FString InNewSpectateSecret);

	UFUNCTION(BlueprintCallable, Category = "Discord")
	void SetSteamAppId(const FString IsNewSteamAppId);

	/**
	* public UDiscordObject::StartDiscordTimer
	* Starts elapsed timer in Rich Presence.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")	
	void StartDiscordTimer();

	/**
	* public UDiscordObject::StopDiscordTimer
	* Stops elapsed timer in Rich Presence.
	**/
	UFUNCTION(BlueprintCallable, Category = "Discord")	
	void StopDiscordTimer();

private:

	void Internal_CreateDiscordObject(const FString& InClientID, const bool bRequireDiscordRunning, const bool bStartElapsedTimer);

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return bCanTick; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual TStatId GetStatId() const override { return TStatId(); }
	
};
