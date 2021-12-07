using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProceduralMap : MonoBehaviour
{
    private Object[] objects;
    private List<Object> listObjects = new List<Object>();
    Collider[] hitColliders;

    void Start()
    {
        objects = Resources.LoadAll("Rooms", typeof(GameObject));

        for (int i = 0; i < objects.Length; i++)
        {
            Instantiate(objects[i]);
            listObjects.Add(objects[i]);
        }

        hitColliders = new Collider[listObjects.Count];

        foreach (GameObject go in listObjects)
        {
            Physics.BoxCastAll(go.transform.position, go.transform.localScale / 2f, Vector3.zero);
            //hitColliders = Physics.OverlapBox(go.transform.position, transform.localScale / 2f, Quaternion.identity);
        }

        int j = 0;

        while (j < hitColliders.Length)
        {

            j++;
        }
    }

    void Update()
    {
        
    }
}
