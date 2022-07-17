// Copyright Denis Maximenko, July 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class GMTK2022Target : TargetRules
{
	public GMTK2022Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GMTK2022" } );
	}
}
