using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UpdateHUD_Script : MonoBehaviour
{
    Text text;
    float dir = 0;

    void Start()
    {
        text = GetComponent<Text>();

        dir = Random.Range(-0.015f, 0.015f);
    }

    void Update()
    {
        Vector3 temp = transform.position;
        Vector3 tempScale = transform.localScale;

        temp.x += dir;
        temp.y += 2 * Time.deltaTime;

        tempScale.x += Time.deltaTime;
        tempScale.y += Time.deltaTime;
        tempScale.z += Time.deltaTime;

        Color tempColor = text.color;

        tempColor.a -= 0.001f;

        transform.position = temp;
        transform.localScale = tempScale;
        text.color = tempColor;

        if (tempColor.a <= 0)
        {
            Destroy(this.gameObject);
        }
    }
}
