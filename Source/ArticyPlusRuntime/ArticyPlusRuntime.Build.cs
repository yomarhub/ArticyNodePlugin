using UnrealBuildTool;

public class ArticyPlusRuntime : ModuleRules
{
    public ArticyPlusRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { });
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "InputCore",
            "CoreUObject",
            "Engine",
            "UMG",
            "ArticyRuntime",
            "ArticyTest"
        });
    }
}
