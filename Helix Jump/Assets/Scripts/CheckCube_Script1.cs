using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckCube_Script1 : MonoBehaviour
{
    GameManager GM;
    Material ExMat;

    void Start()
    {
        GM = FindObjectOfType<GameManager>();
        ExMat = GM.ExploMat;
    }

    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        GameObject Collider = collision.gameObject;

        if (Collider.transform.tag == "Player")
        {
            if (GM.chain >= 3)
            {
                Transform parent = transform.parent.parent;

                for (int i = 0; i < parent.childCount; i++)
                {
                    parent.GetChild(i).gameObject.AddComponent<Rigidbody>();

                    Vector3 force = new Vector3(50f, 25f, Random.Range(0, 200));

                    parent.GetChild(i).gameObject.GetComponent<Rigidbody>().AddForce(force);

                    foreach (Transform item in parent.GetChild(i).transform)
                    {
                        Destroy(item.GetComponent<BoxCollider>());
                        Destroy(parent.GetChild(i).gameObject, 1f);
                        item.GetComponent<MeshRenderer>().material = ExMat;
                    }
                }
            }

            Collider.GetComponent<Ball_Script>().ChangeSkillColor();
            GM.chain = 0;
        }
    }
}
