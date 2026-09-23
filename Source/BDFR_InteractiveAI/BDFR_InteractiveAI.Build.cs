using UnrealBuildTool;

public class BDFR_InteractiveAI : ModuleRules
{
    public BDFR_InteractiveAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "GameplayTags",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "DeveloperSettings"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}
