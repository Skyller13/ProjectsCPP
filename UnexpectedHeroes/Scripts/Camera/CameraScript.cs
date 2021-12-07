using Cinemachine;
using System.Collections;
using System.Collections.Generic;
using Kapy.Move;
using UnityEngine;

public class CameraScript : MonoBehaviour
{
    [SerializeField] CinemachineVirtualCamera[] cameras;
    [SerializeField] GameObject targetPrefab;
    private GameObject target;

    private IngameInputMap ingameMap;

    public GameObject player;

    bool wasMoved = true;
    public bool onTarget = false;
    public short position;
    public short oldPos;
    float timer;
    float speedFactor;

    void Start()
    {
        CheckInputMap();

        timer = 0;
        position = 0;
        speedFactor = 10f;
    }

    void OnDestroy()
    {
        ///Inputs Map
        ///
        ingameMap.ONCameraSwitch -= SwitchCamera;

        ///
    }

    private void CheckInputMap()
    {
        if (ingameMap == null)
        {
            ingameMap = FindObjectOfType<IngameInputMap>();

            if (ingameMap != null)
            {
                ingameMap.ONCameraSwitch += SwitchCamera;
            }
        }
    }

    void SwitchCamera(typeDirection _direction)
    {
        if (!wasMoved
            && player.GetComponent<PlayerController>().PlayerAction == PlayerController.currentAction.isBase)
        {
            switch (position)
            {
                case 0:
                    if (_direction == typeDirection.Left)
                    {
                        cameras[0].transform.gameObject.SetActive(false);
                        cameras[1].transform.gameObject.SetActive(true);
                        position++;
                        wasMoved = true;
                    }
                    else if (_direction == typeDirection.Right)
                    {
                        cameras[0].transform.gameObject.SetActive(false);
                        cameras[3].transform.gameObject.SetActive(true);
                        position--;
                        wasMoved = true;
                    }

                    break;
                case 1:
                    if (_direction == typeDirection.Left)
                    {
                        cameras[1].transform.gameObject.SetActive(false);
                        cameras[2].transform.gameObject.SetActive(true);
                        position++;
                        wasMoved = true;
                    }
                    else if (_direction == typeDirection.Right)
                    {
                        cameras[1].transform.gameObject.SetActive(false);
                        cameras[0].transform.gameObject.SetActive(true);
                        position--;
                        wasMoved = true;
                    }

                    break;
                case 2:
                    if (_direction == typeDirection.Left)
                    {
                        cameras[2].transform.gameObject.SetActive(false);
                        cameras[3].transform.gameObject.SetActive(true);
                        position++;
                        wasMoved = true;
                    }
                    else if (_direction == typeDirection.Right)
                    {
                        cameras[2].transform.gameObject.SetActive(false);
                        cameras[1].transform.gameObject.SetActive(true);
                        position--;
                        wasMoved = true;
                    }

                    break;
                case 3:
                    if (_direction == typeDirection.Left)
                    {
                        cameras[3].transform.gameObject.SetActive(false);
                        cameras[0].transform.gameObject.SetActive(true);
                        position++;
                        wasMoved = true;
                    }
                    else if (_direction == typeDirection.Right)
                    {
                        cameras[3].transform.gameObject.SetActive(false);
                        cameras[2].transform.gameObject.SetActive(true);
                        position--;
                        wasMoved = true;
                    }

                    break;
                default:
                    break;
            }
        }
    }

    void ZoomCamera()
    {
        if (ingameMap.isZoom)
        {
            var transposer = cameras[position].GetCinemachineComponent<CinemachineTransposer>();
            var zoom = transposer.m_FollowOffset;

            if (ingameMap.currentZoom == 1)
            {
                zoom.y -= 10 * Time.deltaTime;
                speedFactor -= 5 * Time.deltaTime;
            }
            else if (ingameMap.currentZoom == -1)
            {
                zoom.y += 10 * Time.deltaTime;
                speedFactor += 5 * Time.deltaTime;
            }


            if (zoom.y > 11)
            {
                zoom.y = 11;
            }
            else if (zoom.y < 6.5f)
            {
                zoom.y = 6.5f;
            }

            cameras[position].GetCinemachineComponent<CinemachineTransposer>().m_FollowOffset = zoom;
        }
    }

    void Update()
    {
        CheckInputMap();

        if (player == null)
        {
            player = Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().gameObject;

            for (int i = 0; i < 4; i++)
            {
                cameras[i].m_Follow = Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().transform;
                cameras[i].m_LookAt = Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().transform;
            }
        }

        if (position > 3)
        {
            position = 0;
        }

        if (position < 0)
        {
            position = 3;
        }

        ZoomCamera();

        if (wasMoved)
        {
            timer += Time.deltaTime;
        }

        if (timer > 1.25f)
        {
            timer = 0;
            wasMoved = false;
        }
    }
}
