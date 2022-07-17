// Copyright Denis Maximenko, July 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class GMTK2022EditorTarget : TargetRules
{
	public GMTK2022EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GMTK2022" } );
	}
}
