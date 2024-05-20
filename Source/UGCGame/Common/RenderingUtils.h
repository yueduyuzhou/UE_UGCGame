#pragma once

#include "CoreMinimal.h"
#include "RenderingUtils.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

USTRUCT(BlueprintType)
struct FMinimapCapture
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
		USceneCaptureComponent2D* SceneCapture2DComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
		UTextureRenderTarget2D* RenderTarget;

	FMinimapCapture();

	void Initialize(UWorld* World);
	void Initialize(UWorld* World, FVector Location, FRotator Rotation, float InOrthoWidth, FVector2D TextureSize = FVector2D(1024, 1024));

	void CaptureMiniMapImage(FString SavePath);

	// 加载BMP图片文件
	UTexture2D* LoadBMPImage(const FString& FilePath);
	// 删除BMP图片文件
	bool DeleteBMPFile(const FString& FilePath);

private:
	void CreateRenderTarget(FVector2D TextureSize);

private:
	FVector CaptureLocation;
	FRotator CaptureRotation;
	FVector2D TextureSize;
};