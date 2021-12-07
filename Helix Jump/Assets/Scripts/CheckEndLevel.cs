using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckEndLevel : MonoBehaviour
{
    GameManager GM;

    // Start is called before the first frame update
    void Start()
    {
        GM = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    private void OnCollisionEnter(Collision collision)
    {
        GameObject Collider = collision.gameObject;

        if (Collider.transform.tag == "Player")
        {
            GM.hasWin = true;
            Time.timeScale = 0f;
        }
    }
}
