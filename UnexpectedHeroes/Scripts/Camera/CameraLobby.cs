using System.Collections;
using Cinemachine;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CameraLobby : MonoBehaviour
{
    [SerializeField] CinemachineVirtualCamera camera;
    [SerializeField] private Transform[] places;
    private GameObject player;

    private void Start()
    {
        player = FindObjectOfType<LobbyPlayerControl>().gameObject;
    }

    private void Update()
    {
        int current = player.GetComponent<LobbyPlayerControl>().GetCurrentPlace();

        camera.m_LookAt = places[current].transform;


        //change to new input system
        if (Input.GetButtonDown("A")
        && SceneManager.GetActiveScene().name == "Lobby_quest")
        {
            camera.enabled = false;
        }
    }
}
