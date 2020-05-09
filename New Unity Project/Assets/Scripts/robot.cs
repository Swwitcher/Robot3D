using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class robot : MonoBehaviour
{
	public const int imageSidePx = 480;
	public Camera leftCam;
	public Camera rightCam;

    // Start is called before the first frame update
    void Start()
    {

		leftCam.enabled = true;
		rightCam.enabled = false;


	}


	void Update()
	{

		if (Input.GetKeyDown("space"))
		{

			Texture2D leftText = getText2D(leftCam);
			byte[] leftCamBytes = leftText.EncodeToJPG();
			Destroy(leftText);
			File.WriteAllBytes(Application.dataPath + "testL.jpeg", leftCamBytes);

			Texture2D rightText = getText2D(rightCam);
			byte[] rightCamBytes = rightText.EncodeToJPG();
			Destroy(rightText);
			File.WriteAllBytes(Application.dataPath + "testR.jpeg", rightCamBytes);
		}
	}


	private Texture2D getText2D(Camera mCamera)
	{
		int mWidth = imageSidePx;
		int mHeight = imageSidePx;
		Rect rect = new Rect(0, 0, mWidth, mHeight);
		RenderTexture renderTexture = new RenderTexture(mWidth, mHeight, 24);
		Texture2D screenShot = new Texture2D(mWidth, mHeight, TextureFormat.RGBA32, false);

		mCamera.targetTexture = renderTexture;
		mCamera.Render();

		RenderTexture.active = renderTexture;
		screenShot.ReadPixels(rect, 0, 0);

		mCamera.targetTexture = null;
		RenderTexture.active = null;

		Destroy(renderTexture);
		renderTexture = null;
		return screenShot;
	}

	private byte[] textToByte(Texture2D text)
	{
		Color[] colormap = text.GetPixels();
		byte[] res = new byte[imageSidePx * imageSidePx];
		for (int i = 0; i < imageSidePx; i++)
		{
			res[i] = System.Convert.ToByte(colormap[i].grayscale * 100.0);
		}
		return res;
	}
}
