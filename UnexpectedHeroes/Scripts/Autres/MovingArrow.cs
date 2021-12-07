using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingArrow : MonoBehaviour
{
    Vector3 min;
    Vector3 max;
    Vector3 current;
    Vector3 tilePos;
    Vector3 offset;
    Vector3 staticOffset;

    float speed;
    bool isAscending;

    void Start()
    {
        isAscending = true;
        speed = 1.25f;
        current = transform.position;
        tilePos = transform.parent.GetChild(0).transform.position;
        offset = transform.position - tilePos;
        staticOffset = transform.position - tilePos;
    }

    void Update()
    {
        max = staticOffset + new Vector3(0, 0.3f, 0);
        min = staticOffset - new Vector3(0, 0.3f, 0);

        if (!isAscending)
        {
            offset.y -= speed * Time.deltaTime;
        }
        else
        {
            offset.y += speed * Time.deltaTime;
        }

        if (offset.y > max.y)
        {
            isAscending = false;
        }

        if (offset.y < min.y)
        {
            isAscending = true;
        }

        current.x = transform.parent.transform.position.x;
        current.z = transform.parent.transform.position.z;
        current.y = transform.parent.transform.position.y + offset.y;

        transform.position = current;
    }
}
