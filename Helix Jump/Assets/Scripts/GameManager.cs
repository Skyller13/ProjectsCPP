using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UIElements;

public class GameManager : MonoBehaviour
{
    private int currentLevel;
    public int score;
    public int maxScore;
    public int chain;
    public int cursor;
    public int endCursor;
    public int nbPlatforms;

    private static bool created = false;
    public bool playerState = true;
    public bool hasWin = false;
    private LevelManager LM;
    [SerializeField] public Material ExploMat;

    private void Awake()
    {
        if (!created)
        {
            LM = FindObjectOfType<LevelManager>();

            DontDestroyOnLoad(this.gameObject);
            playerState = true;
            score = 0;
            nbPlatforms = 0;
            cursor = 0;
            endCursor = LM.currentLevel * 40;
            created = true;
        }
        else
        {
            Destroy(this.gameObject);
        }
    }

    void Start()
    {
        LM = FindObjectOfType<LevelManager>();
        CleanLevel();
    }

    void Update()
    {
        if (!playerState)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                ResetLevel();
            }
        }

        if (hasWin)
        {
            if(Input.GetKeyDown(KeyCode.Space))
            {
                if (LM.currentLevel < 3)
                {
                    LM.currentLevel++;
                }

                NextLevel();
            }
        }

        if (LM.currentLevel != currentLevel)
        {
            currentLevel = LM.currentLevel;
        }
    }

    public void AddScore(int _value)
    {
        score += _value * currentLevel;
    }

    public void CleanLevel()
    {
        playerState = true;
        score = 0;
        chain = 0;
        cursor = 0;
        endCursor = LM.currentLevel * 40;
        nbPlatforms = 0;
    }

    public void ResetLevel()
    {
        Time.timeScale = 1f;
        hasWin = false;
        score = 0;
        cursor = 0;
        nbPlatforms = 0;
        endCursor = LM.currentLevel * 40;
        SceneManager.LoadScene("Helix");
        playerState = true;
    }

    public void NextLevel()
    {
        Time.timeScale = 1f;
        hasWin = false;
        cursor = 0;
        endCursor = LM.currentLevel * 40;
        SceneManager.LoadScene("Helix");
        playerState = true;
        nbPlatforms = 0;
    }
}
