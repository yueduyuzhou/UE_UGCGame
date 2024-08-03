#include "RenderingUtils.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FMinimapCapture::FMinimapCapture()
	: SceneCapture2DComp(nullptr)
	, RenderTarget(nullptr)
	, CaptureLocation(FVector(0.f, 0.f, 5000.f))
	, CaptureRotation(FRotator(-90.f, 0.f, 0.f))
	, TextureSize(FVector2D(1000.f, 1000.f))
{
}

void FMinimapCapture::Initialize(UWorld* InWorld)
{
	ASceneCapture2D* SceneCaptureActor = InWorld->SpawnActor<ASceneCapture2D>(CaptureLocation, CaptureRotation);
	if (SceneCaptureActor)
	{
		SceneCapture2DComp = SceneCaptureActor->GetCaptureComponent2D();
		SceneCapture2DComp->ProjectionType = ECameraProjectionMode::Orthographic;

		SceneCapture2DComp->OrthoWidth = 5600.0f;

		SceneCapture2DComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

		CreateRenderTarget(TextureSize);
	}
}

void FMinimapCapture::Initialize(UWorld* InWorld, FVector InLocation, FRotator InRotation, float InOrthoWidth, FVector2D InTextureSize)
{
	ASceneCapture2D* SceneCaptureActor = InWorld->SpawnActor<ASceneCapture2D>(InLocation, InRotation);
	if (SceneCaptureActor)
	{
		SceneCapture2DComp = SceneCaptureActor->GetCaptureComponent2D();
		SceneCapture2DComp->ProjectionType = ECameraProjectionMode::Orthographic;

		SceneCapture2DComp->OrthoWidth = InOrthoWidth;

		SceneCapture2DComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

		CreateRenderTarget(InTextureSize);
	}
}

void FMinimapCapture::Initialize(UWorld* InWorld, FVector InLocation, FRotator InRotation, ECameraProjectionMode::Type InProjectionType, FVector2D InTextureSize)
{
	ASceneCapture2D* SceneCaptureActor = InWorld->SpawnActor<ASceneCapture2D>(InLocation, InRotation);
	if (SceneCaptureActor)
	{
		SceneCapture2DComp = SceneCaptureActor->GetCaptureComponent2D();
		SceneCapture2DComp->ProjectionType = InProjectionType;

		SceneCapture2DComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

		CreateRenderTarget(InTextureSize);
	}
}

void FMinimapCapture::CreateRenderTarget(FVector2D InTextureSize)
{
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(InTextureSize.X, InTextureSize.Y);
	RenderTarget->UpdateResourceImmediate();
	SceneCapture2DComp->TextureTarget = RenderTarget;
}

void FMinimapCapture::CaptureMiniMapImage(FString SavePath)
{
	if (SceneCapture2DComp && RenderTarget)
	{
		SceneCapture2DComp->CaptureScene();

		FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
		TArray<FColor> Bitmap;
		FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
		RenderTargetResource->ReadPixels(Bitmap, ReadPixelFlags);

		FIntPoint DestSize(RenderTarget->SizeX, RenderTarget->SizeY);
		FFileHelper::CreateBitmap(*SavePath, DestSize.X, DestSize.Y, Bitmap.GetData());
	}
}

UTexture2D* FMinimapCapture::LoadBMPImage(const FString& FilePath)
{
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		return nullptr;
	}

	// 使用IImageWrapperModule获取适当的图像解码器
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());

	// 创建图像解码器
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		return nullptr;
	}

	// 解码图像数据
	TArray<uint8> RawData;
	if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		// 创建2D纹理
		UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
		if (Texture)
		{
			// 将解码后的数据复制到纹理
			void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
			Texture->PlatformData->Mips[0].BulkData.Unlock();
			Texture->UpdateResource();
			return Texture;
		}
	}

	return nullptr;
}

bool FMinimapCapture::DeleteBMPFile(const FString& FilePath)
{
	if (FPaths::FileExists(FilePath))
	{
		// 删除指定路径的文件
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		return PlatformFile.DeleteFile(*FilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class FMinimapCapture] : DeleteBMPFile, FilePath Not Exist"));
	}
	return false;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif