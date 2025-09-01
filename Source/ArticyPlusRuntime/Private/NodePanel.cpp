// Fill out your copyright notice in the Description page of Project Settings.


#include "NodePanel.h"
#include "Engine/Engine.h"
#include "Components/CanvasPanel.h"
#include "Input/Reply.h"

#if false
#include "Components/CanvasPanelSlot.h"

void UNodePanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (CanvasPanel)
	{
		// Center pivot so scale + translation behave nicely
		CanvasPanel->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	}
}

FReply UNodePanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && CanvasPanel && GetCachedWidget().IsValid())
	{
		bDraggingCanvas = true;
		DragStartMouseLocal = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		DragStartTranslation = CanvasPanel->GetRenderTransform().Translation;

		return FReply::Handled().CaptureMouse(GetCachedWidget().ToSharedRef());
	}
	return FReply::Unhandled();
}

FReply UNodePanel::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bDraggingCanvas || !CanvasPanel) return FReply::Unhandled();

	const FVector2D MouseLocal = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	const FVector2D LocalDelta = MouseLocal - DragStartMouseLocal;

	FVector2D Target = DragStartTranslation + LocalDelta;

	// --- Clamp ---
	const FVector2D ParentSize = InGeometry.GetLocalSize();
	const FVector2D ChildSizeUnscaled = CanvasPanel->GetCachedGeometry().GetLocalSize();
	const FVector2D ChildSize = ChildSizeUnscaled * Zoom;

	auto ClampCenteredAxis = [&](float Pos, float Parent, float Child)
		{
			// If canvas is bigger → allow panning, clamp to edges
			if (Child > Parent)
			{
				float HalfRange = (Child - Parent) * 0.5f;
				return FMath::Clamp(Pos, -HalfRange, HalfRange);
			}
			// If canvas is smaller → keep centered
			return 0.f;
		};

	Target.X = ClampCenteredAxis(Target.X, ParentSize.X, ChildSize.X);
	Target.Y = ClampCenteredAxis(Target.Y, ParentSize.Y, ChildSize.Y);

	CanvasPanel->SetRenderTranslation(Target);

	return FReply::Handled();
}

FReply UNodePanel::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && GetCachedWidget().IsValid())
	{
		bDraggingCanvas = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

void UNodePanel::SetZoom(float NewZoom)
{
	if (!CanvasPanel) return;

	Zoom = FMath::Clamp(NewZoom, MinZoom, MaxZoom);

	FWidgetTransform Transform = CanvasPanel->GetRenderTransform();
	Transform.Scale = FVector2D(Zoom, Zoom);
	CanvasPanel->SetRenderTransform(Transform);
}

#else
void UNodePanel::ScaleCanvas(const float ScaleFactor)
{
	if (!CanvasPanel) return;
	UE_LOG(LogTemp, Log, TEXT("Scaled by %s, Current Scale : %s"), *FString::SanitizeFloat(ScaleFactor), *FString::SanitizeFloat(CanvasPanel->RenderTransform.Scale.X));
	// Get the current scale of the CanvasPanel
	FVector2D NewScale = FVector2D::One() * ScaleFactor;
	// Set the new scale to the CanvasPanel
	CanvasPanel->SetRenderScale(NewScale);
}

FReply UNodePanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Center and scale the CanvasPanel
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		ZoomCount++;
		ScaleCanvas(1.0f + (ZoomCount % 5));
		//CanvasPanel->SetRenderTranslation(FVector2D::ZeroVector);
	}
	if (InMouseEvent.GetEffectingButton() != MoveButton || !CanvasPanel) return FReply::Unhandled();

	bDraggingCanvas = true;
	// Get Start Position of the Mouse
	DragStartMousePos = InMouseEvent.GetScreenSpacePosition();
	DragStartCanvasPos = CanvasPanel->GetRenderTransform().Translation;
	return FReply::Handled();
}

FReply UNodePanel::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (!InMouseEvent.IsMouseButtonDown(MoveButton) // Check if Move Button is pressed
		|| !bDraggingCanvas // Check if we are currently dragging the canvas
		|| DragStartMousePos.IsZero() // Check if DragStartMousePos is initialized
		|| !CanvasPanel // Check if CanvasPanel is valid
		) return FReply::Unhandled();
	const FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	const FVector2D LocalDragStart = InGeometry.AbsoluteToLocal(DragStartMousePos);

	const FVector2D Delta = LocalMousePos - LocalDragStart;

	// Move the CanvasPanel based on the mouse movement
	CanvasPanel->SetRenderTranslation(DragStartCanvasPos + Delta);

	return FReply::Handled();
}

FReply UNodePanel::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	DragStartMousePos = FVector2D::ZeroVector;
	return FReply::Handled().ReleaseMouseCapture();
}
#endif

#define LOCTEXT_NAMESPACE "NodePanel"
#if WITH_EDITOR
const FText UNodePanel::GetPaletteCategory()
{
	return LOCTEXT("Node", "Node");
}
#endif
#undef LOCTEXT_NAMESPACE