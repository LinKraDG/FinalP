#pragma once

UENUM(BlueprintType)
enum class EStructureType : uint8
{
	None,
	Decorative,
	Production,
	Logistic,
	Organization
};

UENUM(BlueprintType)
enum class EDecorativeStructureClassify : uint8
{
	None,
	Roofs,
	Walls,
	Ceilings,
	Window_Wall
};

UENUM(BlueprintType)
enum class EProductionStructureClassify : uint8
{
	None,
	Extraction,
	Producers,
	Furnaces
};