using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuButtons : MonoBehaviour
{
    bool quitGame = false;
    bool playGame = false;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (quitGame)
        {
            Application.Quit();
            quitGame = false;
        }

        if (playGame)
        {
            SceneManager.LoadScene(0);
        }
    }

    public void PlayGame()
    {
        playGame = true;
    }

    public void QuitGame()
    {
        quitGame = true;
    }
}
