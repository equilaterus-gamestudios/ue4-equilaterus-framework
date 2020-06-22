// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EQFunctionBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT999_API UEQFunctionBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    template<typename TReturnType, typename TParamType>
    static TReturnType ExecuteFunctionByName(const FName& FunctionName, UObject* Object, TParamType* InParams)
    {
		check(Object); 
		UClass* Class = Object->GetClass();
		UFunction* Function = Class->FindFunctionByName(FunctionName, EIncludeSuperFlag::IncludeSuper);
		check(Function);
				
		// Weird stuff, allocate this memory to store the return value.
		uint8* Params = (uint8*)FMemory_Alloca(Function->ParmsSize);
		FMemory::Memzero(Params, Function->ParmsSize);
		
		if (InParams)
		{
			for (TFieldIterator<UProperty> It(Function); It&& It->HasAnyPropertyFlags(CPF_Parm); ++It)
			{
				UProperty* LocalProp = *It;
				checkSlow(LocalProp);
				*LocalProp->ContainerPtrToValuePtr<TParamType>(Params) = *InParams;
			}
		}		

		// Call the function
		Object->ProcessEvent(Function, Params);

		TReturnType Result;
		// Search the return value in the function object
		for (TFieldIterator<UProperty> PropIt(Function, EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			UProperty* Property = *PropIt;
			if (Property->HasAnyPropertyFlags(CPF_ReturnParm))
			{
				uint8* outValueAddr = Property->ContainerPtrToValuePtr<uint8>(Params);
				TReturnType* pReturn = (TReturnType*)outValueAddr;
				Result = *pReturn;
				break;
			}
		}

		for (TFieldIterator<UProperty> It(Function); It&& It->HasAnyPropertyFlags(CPF_Parm); ++It)
		{
			It->DestroyValue_InContainer(Params);
		}

		return Result;
    }
};
