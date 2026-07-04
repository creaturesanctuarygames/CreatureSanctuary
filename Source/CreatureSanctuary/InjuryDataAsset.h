#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InjuryDataAsset.generated.h"

UCLASS(BlueprintType)
class CREATURESANCTUARY_API UInjuryDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Injury")
    FText DisplayName;
};
