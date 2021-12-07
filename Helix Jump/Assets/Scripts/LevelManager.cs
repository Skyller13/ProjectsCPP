using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelManager : MonoBehaviour
{
    private static bool created = false;
    public int currentLevel;
    public int maxLevel;

    private void Awake()
    {
        if (!created)
        {
            DontDestroyOnLoad(this.gameObject);

            currentLevel = 1;
            maxLevel = 1;

            created = true;
        }
        else
        {
            Destroy(this.gameObject);
        }
    }

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (maxLevel < currentLevel)
        {
            maxLevel = currentLevel;
        }
    }

    public bool CanLoadLevel(int _level)
    {
        if (_level <= maxLevel)
        {
            currentLevel = _level;
            Time.timeScale = 1f;
            return true;
        }
        else
        {
            return false;
        }
    }
}
