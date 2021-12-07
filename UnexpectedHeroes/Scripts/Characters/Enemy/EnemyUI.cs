using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyUI : MonoBehaviour
{
    Enemy enemyUnit;
    [SerializeField] Canvas UI;
    [SerializeField] Image HealthBar;
    Camera mainCam;

    // Start is called before the first frame update
    void Start()
    {
        enemyUnit = GetComponent<Enemy>();
        mainCam = Camera.main;
    }

    // Update is called once per frame
    void Update()
    {
        HealthBar.fillAmount = enemyUnit.GetHPPercentage();
        UI.transform.forward = mainCam.transform.forward;
    }
}
