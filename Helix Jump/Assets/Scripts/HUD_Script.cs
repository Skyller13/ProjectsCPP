using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class HUD_Script : MonoBehaviour
{
    [SerializeField] GameObject addScore;
    [SerializeField] Slider slider;

    [SerializeField] Text currentLevelText;
    [SerializeField] Text nextLevelText;
    [SerializeField] GameObject Menu;
    [SerializeField] GameObject AddGo;

    GameManager GM;
    LevelManager LM;
    int tempScore = 0;

    void Start()
    {
        GM = FindObjectOfType<GameManager>();
        LM = FindObjectOfType<LevelManager>();
    }

    void Update()
    {
        transform.GetChild(0).GetComponent<Text>().text = GM.score.ToString();

        //Update cursor
        slider.value = ((float)GM.cursor / (float)GM.endCursor);

        currentLevelText.text = LM.currentLevel.ToString();

        if (LM.currentLevel < 3)
        {
            nextLevelText.text = (LM.currentLevel + 1).ToString();
        }
        else
        {
            nextLevelText.text = LM.currentLevel.ToString();
        }

        if (!GM.playerState)
        {
            GameObject death = transform.GetChild(1).gameObject;

            death.SetActive(true);

            if (GM.score > GM.maxScore)
            {
                GM.maxScore = GM.score;

                death.transform.GetChild(1).gameObject.SetActive(true);
                death.transform.GetChild(2).gameObject.SetActive(true);
                death.transform.GetChild(2).gameObject.GetComponent<Text>().text = GM.score.ToString();
            }
        }
        else
        {
            if (Input.GetKeyDown(KeyCode.Escape))
            {
                if (!Menu.activeSelf)
                {
                    Menu.SetActive(true);
                    Time.timeScale = 0f;
                }
            }
        }

        if (GM.playerState)
        {
            int diff = GM.score - tempScore;

            if (diff > 0)
            {
                GameObject go = Instantiate(addScore, AddGo.transform);

                go.GetComponent<Text>().text = "+" + diff.ToString();
            }

            tempScore = GM.score;
        }

        if (GM.hasWin)
        {
            GameObject winObj = transform.GetChild(3).gameObject;

            LevelManager lm = FindObjectOfType<LevelManager>();

            winObj.SetActive(true);
            winObj.transform.GetChild(0).GetComponent<Text>().text = "Level " + lm.currentLevel + " passed !";
        }
    }

    public void Continue()
    {
        if (Menu.activeSelf)
        {
            Menu.SetActive(false);
            Time.timeScale = 1f;
        }
    }

    public void QuitGame()
    {
        SceneManager.LoadScene("LevelSelection", LoadSceneMode.Single);
    }
}
