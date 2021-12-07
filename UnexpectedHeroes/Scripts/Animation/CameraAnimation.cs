using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Cinemachine;

public class CameraAnimation : MonoBehaviour
{
    float ShakeDuration = 0.3f;          // Time the Camera Shake effect will last
    float ShakeAmplitude = 1.2f;         // Cinemachine Noise Profile Parameter
    float ShakeFrequency = 2.0f;         // Cinemachine Noise Profile Parameter

    private float ShakeElapsedTime = 0f;

    // Cinemachine Shake
    CinemachineVirtualCamera VirtualCamera;
   [SerializeField] CinemachineVirtualCamera[] VirtualCameras = new CinemachineVirtualCamera[5];
    private CinemachineBasicMultiChannelPerlin virtualCameraNoise;

    // Use this for initialization
    void Start()
    {
        // Get Virtual Camera Noise Profile
        if (VirtualCamera != null)
            virtualCameraNoise = VirtualCamera.GetCinemachineComponent<Cinemachine.CinemachineBasicMultiChannelPerlin>();
        GetCurrentVirtualCamera();
    }

    // Update is called once per frame
    void Update()
    {
        // TODO: Replace with your trigger
        if (Input.GetKey(KeyCode.K))
        {
            Debug.Log("Moka");
            GetCurrentVirtualCamera();
            ShakeElapsedTime = ShakeDuration;
        }

        // If the Cinemachine componet is not set, avoid update
        if (VirtualCamera != null && virtualCameraNoise != null)
        {
            // If Camera Shake effect is still playing
            if (ShakeElapsedTime > 0)
            {
                // Set Cinemachine Camera Noise parameters
                virtualCameraNoise.m_AmplitudeGain = ShakeAmplitude;
                virtualCameraNoise.m_FrequencyGain = ShakeFrequency;

                // Update Shake Timer
                ShakeElapsedTime -= Time.deltaTime;
            }
            else
            {
                // If Camera Shake effect is over, reset variables
                virtualCameraNoise.m_AmplitudeGain = 0f;
                ShakeElapsedTime = 0f;
            }
        }
    }


    void GetCurrentVirtualCamera()
    {
        foreach (var item in VirtualCameras)
        {
            if (item.gameObject.activeSelf)
            {
                VirtualCamera = item;
                break;
            }
        } 
    }
}
