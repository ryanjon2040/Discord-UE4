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

		PublicIncludePaths.Add(DiscordPath);
		PublicAdditionalLibraries.Add(DiscordLibFile);
		
		PublicDependencyModuleNames.AddRange( new string[] { "Core" } );		
		PrivateDependencyModuleNames.AddRange( new string[] { "CoreUObject", "Engine" } );
	}

	private string DiscordPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "discord-files")); }
	}

	private string DiscordLibFile
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll.lib")); }
	}
}
