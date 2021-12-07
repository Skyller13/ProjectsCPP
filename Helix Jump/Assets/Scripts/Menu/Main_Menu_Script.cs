using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Main_Menu_Script : MonoBehaviour
{
    void Start()
    {
        
    }

    void Update()
    {
        
    }

    public void LeaveGame()
    {
        Application.Quit();
    }

    public void PlayGame()
    {
        SceneManager.LoadScene("LevelSelection", LoadSceneMode.Single);
    }

    public void SelectLevel(int _level)
    {
        LevelManager lm = FindObjectOfType<LevelManager>();

        if (lm.CanLoadLevel(_level))
        {
            SceneManager.LoadScene("Helix", LoadSceneMode.Single);
        }
    }

    public void GoToMainMenu()
    {
        SceneManager.LoadScene("Menu", LoadSceneMode.Single);
    }
}
