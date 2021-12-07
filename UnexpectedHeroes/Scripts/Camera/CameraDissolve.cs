using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class CameraDissolve : MonoBehaviour
{
    private Transform player;

    private void Start()
    {
        player = Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().transform;
    }

    private void Update()
    {
        if (player != null && Camera.main != null)
        {
            RaycastHit hit;

            float dist = Vector3.Distance(Camera.main.transform.position, player.transform.position);

            Physics.Raycast(Camera.main.transform.position, Vector3.forward, out hit, dist);

            if (hit.collider.gameObject != null
            && hit.collider != player
            && hit.collider != Camera.main)
            {
                //Debug.Log(hit.collider.name);//TODO
            }
        }
    }
}
