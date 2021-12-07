using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnPlatform_Script : MonoBehaviour
{
    GameManager GM;
    bool havePoint = false;
    int nbPlatform;

    [SerializeField] GameObject Platform;
    [SerializeField] GameObject BadPlatform;
    [SerializeField] GameObject endPlatform;
    Transform ball;

    void Start()
    {
        GM = FindObjectOfType<GameManager>();
        ball = FindObjectOfType<Ball_Script>().transform;

        for (int i = 0; i < transform.childCount; i++)
        {
            Destroy(transform.GetChild(i).gameObject);
        }

        if (GM.nbPlatforms == GM.endCursor - 1)
        {
            CreateEndPlatforms();
            GM.nbPlatforms++;
        }
        else
        {
            CreatePlatforms();
            GM.nbPlatforms++;
        }
    }

    void CreatePlatforms()
    {
        nbPlatform = Random.Range(2, 4);

        for (int i = 0; i < nbPlatform; i++)
        {
            //check random platform
            int rand = Random.Range(0, 10);
            GameObject tempGo;

            if (rand < 8)
            {
                tempGo = Platform;
            }
            else
            {
                if (GM.nbPlatforms == 0)
                {
                    tempGo = Platform;
                }
                else
                {
                    tempGo = BadPlatform;
                }
            }

            //instantiate
            Quaternion tempQuat = new Quaternion();

            tempQuat = Quaternion.Euler(0, Random.Range(0f, 360f), 0);

            GameObject go = Instantiate(tempGo, transform.position, tempQuat, transform);
        }
    }

    void CreateEndPlatforms()
    {
        GameObject tempGo = endPlatform;

        //instantiate
        Quaternion tempQuat = new Quaternion();

        tempQuat = Quaternion.Euler(0, Random.Range(0f, 360f), 0);

        GameObject go = Instantiate(tempGo, transform.position, tempQuat, transform);
    }

    void Update()
    {
        if (!havePoint
            && GM.playerState)
        {
            if (ball.transform.position.y < transform.position.y)
            {
                if (GM.chain >= 3)
                {
                    GM.AddScore(1 + GM.chain);
                }
                else
                {
                    GM.AddScore(1);
                }

                GM.cursor++;
                GM.chain += 1;
                havePoint = true;

                for (int i = 0; i <transform.childCount; i++)
                {
                    transform.GetChild(i).gameObject.AddComponent<Rigidbody>();

                    Vector3 force = new Vector3(50f, 25f, Random.Range(0, 200));
                    
                    transform.GetChild(i).gameObject.GetComponent<Rigidbody>().AddForce(force);

                    foreach (Transform item in transform.GetChild(i))
                    {
                        Destroy(item.GetComponent<BoxCollider>());
                    }

                    Destroy(transform.GetChild(i).gameObject, 1f);
                }
            }
        }
    }
}
