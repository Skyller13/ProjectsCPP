using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Quest : MonoBehaviour
{
    public enum DifficultyQuest
    {
        F,
        E,
        D,
        C,
        B,
        A,
        S,
        X,
    }

    public string name;
    public string description;
    public string goal;
    public string targetName;
    public string dungeonName;
    public int floor;

    private DifficultyQuest difficulty;
    private Image characterImage;
    private Image RewardImage;
    private string rewardName;

    private bool state;

    public void SetActiveQuest()
    {
        state = true;
    }

    public void SetUnactiveQuest()
    {
        state = false;
    }
}
