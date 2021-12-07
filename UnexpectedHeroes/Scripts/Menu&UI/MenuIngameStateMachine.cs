using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class MenuIngameStateMachine : MonoBehaviour
{
    public enum stateMenuIngame
    {
        GLOBAL,
        INVENTORY,
        GLOSSARY,
        OPTION,
        TEAM,
        QUEST,
        WORLDMAP
    }



    [SerializeField] stateMenuIngame currentTypeState = stateMenuIngame.GLOBAL;
    public stateMenuIngame selectedTypeState = stateMenuIngame.GLOBAL;
    [SerializeField] Dictionary<stateMenuIngame, Button> stateDico = new Dictionary<stateMenuIngame, Button>();

    [SerializeField] Button GlobalFirstSelec;
    [SerializeField] Button OptionFirstSelec;
    [SerializeField] Button InventoryFirstSelec;
    [SerializeField] Button GlossaryFirstSelec;
    [SerializeField] Button TeamFirstSelec;
    [SerializeField] Button QuestFirstSelec;

    EventSystem eventSystem;

    // Start is called before the first frame update
    void Start()
    {
        eventSystem = FindObjectOfType<EventSystem>();
       
        stateDico.Add(stateMenuIngame.GLOBAL, GlobalFirstSelec);
        stateDico.Add(stateMenuIngame.OPTION, OptionFirstSelec);
        stateDico.Add(stateMenuIngame.INVENTORY, InventoryFirstSelec);
        stateDico.Add(stateMenuIngame.GLOSSARY, GlossaryFirstSelec);
        stateDico.Add(stateMenuIngame.TEAM, TeamFirstSelec);
        stateDico.Add(stateMenuIngame.QUEST, QuestFirstSelec);
    }

    // Update is called once per frame
    void Update()
    {
        if (currentTypeState != selectedTypeState)
        {
            eventSystem.SetSelectedGameObject(stateDico[selectedTypeState].gameObject);
            currentTypeState = selectedTypeState;
        }
    }
}
