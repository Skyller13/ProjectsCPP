using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DebugInputCharAnim : MonoBehaviour
{
    public enum StateAnim
    {
        IDLE = 0,
        WALK = 1,
        ATTACK = 2,
        CAST = 3,
        HIT = 4,
        DEATH = 5
    }

    [SerializeField] GameObject Character;
    [SerializeField] Animator CharAnim;
    [SerializeField] StateAnim CurrentState = StateAnim.IDLE;

    #region UI
    [SerializeField] Toggle HideUIToggle;
    [Header("Main UI")]
    [SerializeField] Text CurrentAnimTitle;
    [SerializeField] Text SelectorLabel;
    [SerializeField] Dropdown AnimSelector;
    #endregion

    // Start is called before the first frame update
    void Start()
    {
        CharAnim = Character.GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        //Animation settings
        CharAnim.SetInteger("State", (int)CurrentState);

        //UI
        CurrentAnimTitle.gameObject.SetActive(!HideUIToggle.isOn);

        switch (CurrentState)
        {
            case StateAnim.IDLE:
                CurrentAnimTitle.text = "Idle";
                break;
            case StateAnim.WALK:
                CurrentAnimTitle.text = "Walk";
                break;
            case StateAnim.ATTACK:
                CurrentAnimTitle.text = "Attack";
                break;
            case StateAnim.CAST:
                CurrentAnimTitle.text = "Cast";
                break;
            case StateAnim.HIT:
                CurrentAnimTitle.text = "Hit";
                break;
            case StateAnim.DEATH:
                CurrentAnimTitle.text = "Death";
                break;
            default:
                CurrentAnimTitle.text = "Error !";
                break;
        }

        CurrentState = (StateAnim)AnimSelector.value;
    }
}
