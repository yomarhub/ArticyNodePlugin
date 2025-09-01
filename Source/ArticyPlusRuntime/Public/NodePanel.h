// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NodePanel.generated.h"

class UCanvasPanel;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class ARTICYPLUSRUNTIME_API UNodePanel : public UUserWidget
{
	GENERATED_BODY()

#if false
public:
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	//UFUNCTION(BlueprintCallable, Category = "NodePanel")
	void SetZoom(float NewZoom);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	bool bDraggingCanvas = false;
	FVector2D DragStartMouseLocal = FVector2D::ZeroVector;
	FVector2D DragStartTranslation = FVector2D::ZeroVector;

	float Zoom = 1.f;          // current zoom factor
	float MinZoom = 0.25f;     // zoom out limit
	float MaxZoom = 2.0f;      // zoom in limit
};
#endif
bool bDraggingCanvas = false;
FVector2D DragStartMousePos;
FVector2D DragStartCanvasPos;
int ZoomCount = 0;

public:
	FKey MoveButton = EKeys::LeftMouseButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	/**
	 * Scales the canvas by the given scale factor.
	 * @param ScaleFactor The factor by which to scale the canvas.
	 */
	UFUNCTION(BlueprintCallable, Category = "NodePanel")
	void ScaleCanvas(const float ScaleFactor = 5.0f);

#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface
#endif

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};

