using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tower_Script : MonoBehaviour
{
    GameManager gm;
    bool isDone = false;
    GameObject ball;

    [SerializeField] Transform Spawn;
    [SerializeField] GameObject Tower;

    void Start()
    {
        gm = FindObjectOfType<GameManager>();
        ball = FindObjectOfType<Ball_Script>().gameObject;
    }

    void Update()
    {
        if (gm.playerState)
        {
            if (ball.transform.position.y <= transform.position.y
                && !isDone
                && gm.nbPlatforms < gm.endCursor - 2)
            {
                //create a new tower
                GameObject gameObject = Instantiate(Tower, Spawn.position, Quaternion.identity);
                gameObject.name = "Tower";
                isDone = true;
            }

            if (ball.transform.position.y < Spawn.transform.position.y)
            {
                Destroy(transform.parent.gameObject);
            }
        }

        if (gm.cursor == 0)
        {
            gm.nbPlatforms = 0;
        }
    }
}
