// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319

using UnrealBuildTool;
using System.IO;

public class DiscordUE4 : ModuleRules
{
	public DiscordUE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Directory.Exists(DiscordPath) == false)
		{
			throw new BuildException("discord files not found. Check it exists here: " + DiscordPath);
		}

		if (File.Exists(DiscordLibFile) == false)
		{
			throw new BuildException("discord_game_sdk.dll.lib was not found. Check it exists here: " + DiscordLibFile);
		}

		if (File.Exists(DiscordDllFile) == false)
		{
			throw new BuildException("discord_game_sdk.dll was not found. Check it exists here: " + DiscordDllFile);
		}

		PublicIncludePaths.Add(DiscordPath);
		
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "Projects" } );		
		PrivateDependencyModuleNames.AddRange( new string[] { "CoreUObject", "Engine" } );

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDelayLoadDLLs.Add("discord_game_sdk.dll");
			PublicAdditionalLibraries.Add(DiscordLibFile);
			RuntimeDependencies.Add(DiscordDllFile);
		}
	}

	private string DiscordPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "discord-files")); }
	}

	private string DiscordLibFile
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll.lib")); }
	}

	private string DiscordDllFile
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll")); }
	}
}
