#pragma once

#include "CoreMinimal.h"
#include "Construction/Machine/MachineBase.h"
#include "Structs/RecipeData.h"
#include "MachineRecipe.generated.h"

class UDatabaseSubsystem;
class UTechTreeSubsystem;

UCLASS(Blueprintable, BlueprintType)
class FINALP_API AMachineRecipe : public AMachineBase
{
	GENERATED_BODY()
	
public:
	AMachineRecipe();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineRecipe")
	FName activeRecipeId;

	UFUNCTION(BlueprintPure)
	FName GetActiveRecipe() const { return activeRecipeId; }

	UFUNCTION(BlueprintCallable)
	bool SetRecipe(FName recipeId);

	UFUNCTION(BlueprintCallable)
	void GetAvailableRecipes(TArray<FName>& outRecipeIds) const;

	UFUNCTION(BlueprintPure)
	const FRecipeData& GetRecipeData() const { return currentRecipe; }

	UFUNCTION(BlueprintPure)
	bool HasValidRecipe() const { return bHasRecipe; }

	virtual bool CanProcess_Implementation() override;
	virtual void Process_Implementation() override;

protected:
	virtual void BeginPlay() override;

	bool bHasRecipe = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MachineRecipe")
	FRecipeData currentRecipe;

	void RefreshRecipe();

	int32 CountInput(FName itemId) const;

	void ConsumeInput(FName itemId, int32 amount);

	UDatabaseSubsystem* GetItemDatabase() const;

	UTechTreeSubsystem* GetTechTree() const;
};
